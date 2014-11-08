//
//								achorus.cpp
//

#include <stdio.h>
#include <string.h>
#include <math.h>

#ifndef __achorus__
#include "achorus.h"
#endif

#ifndef __sdeditor__
#include "editor/sdeditor.h"
#endif

//
//								AChorusProgram 
//
AChorusProgram::AChorusProgram ()
{
	// default Program Values
	paramSweepRate = 0.0f;
	paramWidth = 0.3f;
	paramFeedback = 0.0f;
	paramDelay = 0.2f;
	paramMixMode = 0.0f;
	strcpy (name, "Init");
}

//
//								AChorus 
//
AChorus::AChorus(audioMasterCallback audioMaster)
	: AudioEffectX (audioMaster, kNumPrograms, kNumParams)
{
	// init
	programs = new AChorusProgram[numPrograms];

	// make sure all instance vars are init'd to some known value
	_paramSweepRate = 0.2f;
	_paramWidth = 0.3f;
	_paramFeedback = 0.0f;
	_paramDelay = 0.2f;
	_paramMixMode = 0.0f;
	_sweepRate = 0.2;
	_feedback = 0.0;
	_feedbackPhase = 1.0;
	_sweepSamples = 0;
	_delaySamples = 22;
	setSweep();
	_mixMode = 0;
	_fp = 0;
	_sweep = 0.0;
	_mixLeftWet =
	_mixLeftDry =
	_mixRightWet =
	_mixRightDry = 0.5f;

	// set initial program (defaults)
	if (programs)
		setProgram (0);

	setNumInputs (2);	// pseudo-mono input
	setNumOutputs (2);	// pseudo-mono output

	setUniqueID ('tY6U');	// this should be unique, use the Steinberg web page for plugin Id registration

	// create the editor
	editor = new SDEditor (this);

	// allocate the buffer
	_buf = new double[BSZ];

	resume ();		// flush buffer
}

//
//								~AChorus 
//
AChorus::~AChorus ()
{
	if (programs)
		delete[] programs;
	if( _buf )
		delete[] _buf;
}

//
//								setProgram
//
void AChorus::setProgram (VstInt32 program)
{
	AChorusProgram* ap = &programs[program];

	curProgram = program;
	setParameter (kRate, ap->paramSweepRate);	
	setParameter (kWidth, ap->paramWidth);
	setParameter (kFeedback, ap->paramFeedback);
	setParameter (kDelay, ap->paramDelay);
	setParameter (kMixMode, ap->paramMixMode);
}

//
//								setRate
//
void AChorus::setRate (float rate)
{
	_paramSweepRate = rate;
	programs[curProgram].paramSweepRate = rate;

	// map into param onto desired sweep range with log curve
	_sweepRate = pow(10.0,(double)_paramSweepRate);
	_sweepRate  -= 1.0;
	_sweepRate  *= 1.1f;
	_sweepRate  += 0.1f;

	// finish setup
	setSweep();
}


//
//								setWidth
//
//	Maps 0.0-1.0 input to calculated width in samples from 0ms to 50ms
//
void AChorus::setWidth (float v)
{
	_paramWidth = v;
	programs[curProgram].paramWidth = v;

	// map so that we can spec between 0ms and 50ms
	_sweepSamples = ROUND(v * 0.05 * SAMPLE_RATE);

	// finish setup
	setSweep();
}

//
//								setDelay
//
//	Expects input from 0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0
//
void AChorus::setDelay (float v)
{
	_paramDelay = v;
	programs[curProgram].paramDelay = v;

	// make onto desired values applying log curve
	double delay = pow(10.0, (double)v * 2.0)/1000.0;		// map logarithmically and convert to seconds
	_delaySamples = ROUND(delay * SAMPLE_RATE);

	// finish setup
	setSweep();
}

//
//								setSweep
//
//	Sets up sweep based on rate, depth, and delay as they're all interrelated
//	Assumes _sweepRate, _sweepSamples, and _delaySamples have all been set by
//	setRate, setWidth, and setDelay
//
void AChorus::setSweep()
{
	// calc # of samples per second we'll need to move to achieve spec'd sweep rate
	_step = (double)(_sweepSamples * 2.0 * _sweepRate) / (double)SAMPLE_RATE;
	if( _step <= 1.0 )
	{
		printf( "_step out of range: %f\n", _step );
	}

	// calc min and max sweep now
	_minSweepSamples = _delaySamples;
	_maxSweepSamples = _delaySamples + _sweepSamples;

	// set intial sweep pointer to midrange
	_sweep = (_minSweepSamples + _maxSweepSamples) / 2;
}


//
//								setFeedback
//
void AChorus::setFeedback(float v)
{
	_paramFeedback = v;
	programs[curProgram].paramFeedback = v;
	_feedback = v;
}


//
//								setMixMode
//
//	Expects input 0.0, 0.2, 0.4, 0.6, 0.8 and maps to the five supported mix modes
//
void AChorus::setMixMode (float v)
{
	_paramMixMode = v;
	programs[curProgram].paramMixMode = v;
	_mixMode = (int)(v * NUM_MIX_MODES);
	switch(_mixMode)
	{
	case kMixMono:
	default:
		_mixLeftWet = _mixRightWet = 1.0;
		_mixLeftDry = _mixRightDry = 1.0f;
		_feedbackPhase = 1.0;
		break;
	case kMixWetOnly:
		_mixLeftWet = _mixRightWet = 1.0f;
		_mixLeftDry = _mixRightDry = 1.0;
		_feedbackPhase = -1.0;
		break;
	case kMixWetLeft:
		_mixLeftWet = 1.0f;
		_mixLeftDry = 0.0f;
		_mixRightWet = 0.0f;
		_mixRightDry = 1.0f;
		break;
	case kMixWetRight:
		_mixLeftWet = 0.0f;
		_mixLeftDry = 1.0f;
		_mixRightWet = 1.0f;
		_mixRightDry = 0.0f;
		break;
	case kMixStereo:
		_mixLeftWet = 1.0f;
		_mixLeftDry = 1.0f;
		_mixRightWet = -1.0f;
		_mixRightDry = 1.0f;
		break;
	}
}



//
//								setProgramName
//
void AChorus::setProgramName (char *name)
{
	strcpy (programs[curProgram].name, name);
}

//
//								getProgramName
//
void AChorus::getProgramName (char *name)
{
	if (!strcmp (programs[curProgram].name, "Init"))
		sprintf (name, "%s %d", programs[curProgram].name, curProgram + 1);
	else
		strcpy (name, programs[curProgram].name);
}

//
//								getProgramNameIndexed
//
bool AChorus::getProgramNameIndexed (VstInt32 category, VstInt32 index, char* text)
{
	if (index < kNumPrograms)
	{
		strcpy (text, programs[index].name);
		return true;
	}
	return false;
}

//
//								resume
//
void AChorus::resume ()
{
	AudioEffectX::resume ();
}

//
//								setParameter
//
void AChorus::setParameter (VstInt32 index, float value)
{
	AChorusProgram* ap = &programs[curProgram];

	switch (index)
	{
		case kRate:    setRate(value);					break;
		case kWidth:   setWidth(value);					break;
		case kFeedback: setFeedback(value);				break;
		case kDelay:	setDelay(value);				break;
		case kMixMode: setMixMode(value);				break;
	}
}

//
//								getParameter
//
float AChorus::getParameter (VstInt32 index)
{
	float v = 0;

	switch (index)
	{
		case kRate :    v = _paramSweepRate;	break;
		case kWidth:	v = _paramWidth;		break;
		case kFeedback: v = _paramFeedback;		break;
		case kDelay:	v = _paramDelay;		break;
		case kMixMode:	v = _paramMixMode;		break;
	}
	return v;
}

//
//								getParameterName
//
void AChorus::getParameterName (VstInt32 index, char *label)
{
	switch (index)
	{
		case kRate:    strcpy (label, "Rate");			break;
		case kWidth:   strcpy (label, "Width");			break;
		case kFeedback: strcpy(label, "Feedback");		break;
		case kDelay:	strcpy (label, "Delay");		break;
		case kMixMode: strcpy (label, "Mix Mode");		break;
	}
}

//
//								getParameterDisplay
//
void AChorus::getParameterDisplay (VstInt32 index, char *text)
{
	char buf[64];
	switch (index)
	{
	case kRate:    
		sprintf( buf, "%2.1f", _sweepRate );
		strcpy( text, buf );
		break;
	case kWidth:
		sprintf( buf, "%2.0f", _paramWidth * 100.0 );
		strcpy( text, buf );
		break;
	case kFeedback:
		sprintf( buf, "%2.0f", _feedback * 100.0 );
		strcpy( text,buf );
		break;
	case kDelay:
		sprintf( buf, "%d", _paramDelay);
		break;
	case kMixMode:
		switch(_mixMode)
		{
		case kMixMono:
			strcpy( text, "mono" );
			break;
		case kMixWetOnly:
			strcpy( text, "wet only" );
			break;
		case kMixWetLeft:
			strcpy( text, "wet left" );
			break;
		case kMixWetRight:
			strcpy( text, "wet right" );
			break;
		case kMixWetLeft75:
			strcpy( text, "wet left 75%" );
			break;
		case kMixWetRight75:
			strcpy( text, "wet right 75%" );
			break;
		case kMixStereo:
			strcpy( text, "stereo" );
			break;
		}
		break;
	}
}

//
//								getParameterLabel
//
void AChorus::getParameterLabel (VstInt32 index, char *label)
{
	switch (index)
	{
		case kRate:		strcpy (label, "Hz");		break;
		case kWidth:	strcpy( label, "%");		break;
		case kFeedback:	strcpy( label, "%");		break;
		case kDelay:	strcpy( label, "");			break;
		case kMixMode:	strcpy( label, "");			break;
	}
}

//
//								getEffectName
//
bool AChorus::getEffectName (char* name)
{
	strcpy (name, "Chorus");
	return true;
}

//
//								getProductString
//
bool AChorus::getProductString (char* text)
{
	strcpy (text, "Chorus");
	return true;
}

//
//								getVendorString
//
bool AChorus::getVendorString (char* text)
{
	strcpy (text, "DarkVapor");
	return true;
}

//
//								processReplacing
//
void AChorus::processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames)
{
	float* in1 = inputs[0];
	float* in2 = inputs[1];
	float* out1 = outputs[0];
	float* out2 = outputs[1];

	for( int i = 0; i < sampleFrames; i++ )
	{
		// assemble mono input value and store it in circle queue
		float inval = (in1[i] + in2[i]) / 2.0f;
		_buf[_fp] = inval;
		_fp = (_fp + 1) & (BSZ-1);

		// build the two emptying pointers and do linear interpolation
		int ep1, ep2;
		double w1, w2;
		double ep = _fp - _sweep;
		MODF(ep, ep1, w2);
		ep1 &= (BSZ-1);
		ep2 = ep1 + 1;
		ep2 &= (BSZ-1);
		w1 = 1.0 - w2;
		double outval = _buf[ep1] * w1 + _buf[ep2] * w2;

		// develop output mix
		out1[i] = (float)PIN(_mixLeftDry * inval + _mixLeftWet * outval, -0.99, 0.99);
		out2[i] = (float)PIN(_mixRightDry * inval + _mixRightWet * outval,-0.99, 0.99);

		// increment the sweep
		_sweep += _step;
		if( _sweep >= _maxSweepSamples || _sweep <= _minSweepSamples )
		{
			_step = -_step;
		}

	}

}
