//
//								aguitar.cpp
//

#include <stdio.h>
#include <string.h>
#include <math.h>

#include "filter.h"
#include "CurveApprox.h"
#ifndef __aguitar__
#include "aguitar.h"
#endif

#ifndef __sdeditor__
#include "editor/sdeditor.h"
#endif

#define	ATTACK_MS		1
#define RELEASE_MS		250
#define ATTACK_RATE		(1.0 / (ATTACK_MS / 1000.0))
#define RELEASE_RATE 	(1.0 / (RELEASE_MS / 1000.0))
#define ATTACK_DELTA	(ATTACK_RATE / SAMPLE_RATE)
#define RELEASE_DELTA	(RELEASE_RATE / SAMPLE_RATE)

void outputDebugInt(char *msg, int val)
{
	char buf[256];
	sprintf(buf, "%s %d\n", msg, val);
	::OutputDebugString(buf);
}

void outputDebugDouble(char *msg, double val)
{
	char buf[256];
	sprintf(buf, "%s %f\n", msg, val);
	::OutputDebugString(buf);
}

//----------------------------------------------------------------------------- 
AGuitarProgram::AGuitarProgram ()
{
	// default Program Values
	paramTone= 0.25f;
	strcpy (name, "Init");
}

//-----------------------------------------------------------------------------
AGuitar::AGuitar (audioMasterCallback audioMaster)
	: _limiter(NULL), _chorus(NULL), _phaser(NULL), _highPass(NULL), AudioEffectX (audioMaster, kNumPrograms, kNumParams)
{
	// init
	
	programs = new AGuitarProgram[numPrograms];

	if (programs)
		setProgram (0);

	setNumInputs (2);	// fake stereo input
	setNumOutputs (2);	// fake stereo output

	_controlFilter = new OnePoleLowPassFilter(20.0);
	_highPass = new TwoPoleHighPassFilter(5000);
	_limiter = new Limiter(250.0);
	_phaser = new Phaser();
	_chorus = new Chorus();

	// now set up the program
	double inputs[9] =      { 0.0, 0.125, 0.25, 0.375, 0.5,  0.625, 0.75, 0.875,   1.0 };
	double limiterGain[9] = { 2.0, 1.0,   15.0, 40.0,  1.0,  2.0,   15.0,  30.0,   40.0};
	double filterGain[9] =  { 0.0, 1.5,    2.0, 3.0,   4.0,  1.0,   0.5,   0.0,    0.0 };
	double phaserGain[9] =  { 0.0, 0.0,    0.0, 0.0,   0.0,  0.0,   1.0,   1.0,    1.0 };
	double overdrive[9] =   { 0.7, 0.4,    0.7, 0.7,   4.0,  7.0,   10.0,  20.0,   30.0 };

	double inGain[10] =  { 0.0, 0.125, 0.25, 0.375, 0.4375, 0.5, 0.625, 0.75, 0.875, 1.0 };
	double outGain[10] = { 5.0, 3.0,   2.0,  1.5,   0.4,    1.0, 0.36,  0.25, 0.2,   0.17 };
	_caLimiterGain = new CurveApprox(9, inputs, limiterGain, "limiter");
	_caFilterGain = new CurveApprox(9, inputs, filterGain, "filter");
	_caPhaserGain = new CurveApprox(9, inputs, phaserGain, "phaser");
	_caOverdrive = new CurveApprox(9, inputs, overdrive, "overdrive");
	_caOutGain = new CurveApprox(10, inGain, outGain, "output");

	setUniqueID ('a67R');	// this should be unique, use the Steinberg web page for plugin Id registration

	// create the editor
	editor = new SDEditor(this);

	resume();		// flush buffer
}

//------------------------------------------------------------------------
AGuitar::~AGuitar ()
{
	if (programs)
	{
		delete[] programs;
	}
	delete _highPass; 
	delete _limiter;
	delete _phaser;
	delete _chorus;
}

//------------------------------------------------------------------------
void AGuitar::setProgram (VstInt32 program)
{
	outputDebugInt("setProgram: ", program);
	AGuitarProgram* ap = &programs[program];

	curProgram = program;
	setParameter (kTone, ap->paramTone);	
}

//------------------------------------------------------------------------
void AGuitar::setTone(float v)
{
	_paramTone = v;
	programs[curProgram].paramTone = v;
	outputDebugDouble("setTone:", v);
	_tone = v;
}

//------------------------------------------------------------------------


//------------------------------------------------------------------------
void AGuitar::setProgramName (char *name)
{
	strcpy (programs[curProgram].name, name);
}

//------------------------------------------------------------------------
void AGuitar::getProgramName (char *name)
{
	if (!strcmp (programs[curProgram].name, "Init"))
		sprintf (name, "%s %d", programs[curProgram].name, curProgram + 1);
	else
		strcpy (name, programs[curProgram].name);
}

//-----------------------------------------------------------------------------------------
bool AGuitar::getProgramNameIndexed (VstInt32 category, VstInt32 index, char* text)
{
	if (index < kNumPrograms)
	{
		strcpy (text, programs[index].name);
		return true;
	}
	return false;
}

//------------------------------------------------------------------------
void AGuitar::resume ()
{
	AudioEffectX::resume ();
}

//------------------------------------------------------------------------
void AGuitar::setParameter (VstInt32 index, float value)
{
	outputDebugInt("setParam: ", index);
	outputDebugDouble("param: ", value);

	AGuitarProgram* ap = &programs[curProgram];

	switch (index)
	{
		case kTone:    
			setTone(value);					
			break;
	}
}

//------------------------------------------------------------------------
float AGuitar::getParameter (VstInt32 index)
{
	float v = 0;

	switch (index)
	{
		case kTone:    
			v = _paramTone;	
			break;
	}
	return v;
}

//------------------------------------------------------------------------
void AGuitar::getParameterName (VstInt32 index, char *label)
{
	switch (index)
	{
		case kTone:    
			strcpy (label, "Tone");		
			break;
	}
}

//------------------------------------------------------------------------
void AGuitar::getParameterDisplay (VstInt32 index, char *text)
{
	char buf[64];
	switch (index)
	{
		case kTone:    
			sprintf( buf, "%2.1f", _tone);
			strcpy( text, buf );
			break;
	}
}

//------------------------------------------------------------------------
void AGuitar::getParameterLabel (VstInt32 index, char *label)
{
	switch (index)
	{
		case kTone:    
			strcpy (label, "");	
			break;
	}
}

//------------------------------------------------------------------------
bool AGuitar::getEffectName (char* name)
{
	strcpy (name, "Guitar Processor");
	return true;
}

//------------------------------------------------------------------------
bool AGuitar::getProductString (char* text)
{
	strcpy (text, "Guitar Processor");
	return true;
}

//------------------------------------------------------------------------
bool AGuitar::getVendorString (char* text)
{
	strcpy (text, "DarkVapor");
	return true;
}

//---------------------------------------------------------------------------
void AGuitar::processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames)
{
	float* in1 = inputs[0];
	float* in2 = inputs[1];
	float* out1 = outputs[0];
	float* out2 = outputs[1];
	double inval;

	for( int i = 0; i < sampleFrames; i++ )
	{
		// get filtered control
		double control = _controlFilter->Process(_tone);

		// limiter
		inval = in1[i] + in2[i];
		inval *= _caLimiterGain->Map(control);
		double outval = _limiter->Process(inval);

		// apply hipass 
		outval += _caFilterGain->Map(control) * _highPass->Process(outval);

		// blend some phaser
		outval += _caPhaserGain->Map(control) * _phaser->Process(outval);

		// use the compressed output to drive some distortion
		outval *= _caOverdrive->Map(control);
		outval = atan(outval);
		outval *= _caOutGain->Map(control);
		out1[i] = (float)outval;
		out2[i] = (float)_chorus->Process(outval);
	}
}
