//
//								aphaser.cpp
//

#include <stdio.h>
#include <string.h>
#include <math.h>

#ifndef __aphaser__
#include "aphaser.h"
#endif

#ifndef __sdeditor__
#include "editor/sdeditor.h"
#endif

//
//								APhaserProgram 
//
APhaserProgram::APhaserProgram ()
{
	// default Program Values
	paramSweepRate = 0.2f;
	paramWidth = 1.0f;
	paramFeedback = 0.0f;
	paramStages = 0.2f;
	paramMixMode = 0.0f;
	strcpy (name, "Init");
}

//
//								APhaserProgram 
//
APhaserProgram::APhaserProgram (double sweep, double width, double feedback, double stages, double mixMode, char *progName)
{
	// default Program Values
	paramSweepRate = sweep;
	paramWidth = width;
	paramFeedback = feedback;
	paramStages = stages;
	paramMixMode = mixMode;
	strcpy (name, progName);
}

//
//								APhaser 
//
APhaser::APhaser (audioMasterCallback audioMaster)
	: AudioEffectX (audioMaster, kNumPrograms, kNumParams)
{
	// init
	pPrograms[0] = new APhaserProgram( 0.1, 0.8, 0.0, 0.0, 0.0, "2 Stage");	
	pPrograms[1] = new APhaserProgram( 0.1, 0.8, 0.0, 0.2, 0.0, "4 Stage");	
	pPrograms[2] = new APhaserProgram( 0.1, 0.8, 0.0, 0.4, 0.0, "6 Stage");	
	pPrograms[3] = new APhaserProgram( 0.1, 0.8, 0.0, 0.6, 0.0, "8 Stage");	
	pPrograms[4] = new APhaserProgram( 0.1, 0.8, 0.0, 0.8, 0.0, "10 Stage");	
	pPrograms[5] = new APhaserProgram( 0.1, 0.8, 0.3, 0.2, 0.0, "4 Stage Feedback");	
	pPrograms[6] = new APhaserProgram( 0.1, 0.8, 0.3, 0.6, 0.0, "6 Stage Feedback");	
	pPrograms[7] = new APhaserProgram( 0.1, 0.8, 0.3, 0.8, 0.0, "8 Stage Feedback");	

	if (pPrograms[0])
	{
		setProgram (0);
	}

	setNumInputs (2);	// pseudo-mono input
	setNumOutputs (2);	// pseudo-mono output

	setUniqueID ('Qpe8');	// this should be unique, use the Steinberg web page for plugin Id registration

	// create the editor
	editor = new SDEditor (this);

	resume ();		// flush buffer
}

//
//								~APhaser 
//
APhaser::~APhaser ()
{
	for( int i = 0; i < kNumPrograms; i++)
	{
		APhaserProgram *app = pPrograms[i];
		delete app;
		pPrograms[i] = NULL;
	}
}

//
//								setProgram
//
void APhaser::setProgram (VstInt32 program)
{
	APhaserProgram* ap = pPrograms[program];

	curProgram = program;
	setParameter (kRate, ap->paramSweepRate);	
	setParameter (kWidth, ap->paramWidth);
	setParameter (kFeedback, ap->paramFeedback);
	setParameter (kStages, ap->paramStages);
	setParameter (kMixMode, ap->paramMixMode);
}

//
//								setRate
//
void APhaser::setRate (float rate)
{
	_paramSweepRate = rate;
	pPrograms[curProgram]->paramSweepRate = rate;
	setSweep();
}


//
//								setWidth
//
void APhaser::setWidth (float v)
{
	_paramWidth = v;
	pPrograms[curProgram]->paramWidth = v;
	setSweep();
}

//
//								setFeedback
//
void APhaser::setFeedback(float v)
{
	_paramFeedback = v;
	pPrograms[curProgram]->paramFeedback = v;
	_feedback = v;
}

//
//								setStages
//
//	Expects input from 0.0, 0.2, 0.4, 0.6, 0.8
//
void APhaser::setStages (float v)
{
	_paramStages = v;
	pPrograms[curProgram]->paramStages = v;
	_stages = (int)(v * 10.0 + 2);
}

//
//								setMixMode
//
//	Expects input 0.0, 0.2, 0.4, 0.6, 0.8 and maps to the five supported mix modes
//
void APhaser::setMixMode (float v)
{
	_paramMixMode = v;
	pPrograms[curProgram]->paramMixMode = v;
	_mixMode = (int)(v * 5.0);
	switch(_mixMode)
	{
	case kMixMono:
	default:
		_mixLeftWet = _mixRightWet = 1.0f;
		_mixLeftDry = _mixRightDry = 1.0f;
		_feedbackPhase = 1.0;
		break;
	case kMixMonoMinus:
		_mixLeftWet = _mixRightWet = -1.0f;
		_mixLeftDry = _mixRightDry = 1.0f;
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
//								setSweep
//
//	Sets up sweep based on rate and width
//
void APhaser::setSweep()
{
	// calc the actual sweep rate
	double rate = pow(10.0,(double)_paramSweepRate);
	rate -= 1.0;
	rate *= 1.1f;
	rate += 0.1f;
	_sweepRate = rate;
	double range = _paramWidth * 6.0f;

	// do the rest of the inits here just for fun
	_wp = _minwp = (M_PI * BOTTOM_FREQ) / (double)SAMPLE_RATE;
	_maxwp = (M_PI * BOTTOM_FREQ * range) / (double)SAMPLE_RATE;

	// figure out increment to multiply by each time
	_sweepFactor = pow( (double)range, (double)(_sweepRate / (SAMPLE_RATE / 2.0)) );

	// init the all pass line
	_lx1 = _ly1 =
	_lx2 = _ly2 =
	_lx3 = _ly3 =
	_lx4 = _ly4 = 
	_lx5 = _ly5 = 
	_lx6 = _ly6 =
	_lx7 = _ly7 =
	_lx8 = _ly8 =
	_lx9 = _ly9 = 
	_lx10 = _ly10 = 0.0f;
}


//
//								setProgramName
//
void APhaser::setProgramName (char *name)
{
	strcpy (pPrograms[curProgram]->name, name);
}

//
//								getProgramName
//
void APhaser::getProgramName (char *name)
{
	if (!strcmp (pPrograms[curProgram]->name, "Init"))
		sprintf (name, "%s %d", pPrograms[curProgram]->name, curProgram + 1);
	else
		strcpy (name, pPrograms[curProgram]->name);
}

//
//								getProgramNameIndexed
//
bool APhaser::getProgramNameIndexed (VstInt32 category, VstInt32 index, char* text)
{
	if (index < kNumPrograms)
	{
		strcpy (text, pPrograms[index]->name);
		return true;
	}
	return false;
}

//
//								resume
//
void APhaser::resume ()
{
	AudioEffectX::resume ();
}

//
//								setParameter
//
void APhaser::setParameter (VstInt32 index, float value)
{
	APhaserProgram* ap = pPrograms[curProgram];

	switch (index)
	{
		case kRate:    setRate(value);					break;
		case kWidth:   setWidth(value);					break;
		case kFeedback: setFeedback(value);				break;
		case kStages:  setStages(value);				break;
		case kMixMode: setMixMode(value);				break;
	}
}

//
//								getParameter
//
float APhaser::getParameter (VstInt32 index)
{
	float v = 0;

	switch (index)
	{
		case kRate :    v = _paramSweepRate;	break;
		case kWidth:	v = _paramWidth;		break;
		case kFeedback: v = _paramFeedback;		break;
		case kStages:	v = _paramStages;		break;
		case kMixMode:	v = _paramMixMode;		break;
	}
	return v;
}

//
//								getParameterName
//
void APhaser::getParameterName (VstInt32 index, char *label)
{
	switch (index)
	{
		case kRate:    strcpy (label, "Rate");			break;
		case kWidth:   strcpy (label, "Width");			break;
		case kFeedback: strcpy(label, "Feedback");		break;
		case kStages:  strcpy (label, "Stages");		break;
		case kMixMode: strcpy (label, "Mix Mode");		break;
	}
}

//
//								getParameterDisplay
//
void APhaser::getParameterDisplay (VstInt32 index, char *text)
{
	char buf[256];
	switch (index)
	{
	case kRate:    
		sprintf( buf, "%2.1f", _sweepRate );
		strcpy( text, buf );
		break;
	case kWidth:
		sprintf( buf, "%2.0f", _width * 100.0 );
		strcpy( text, buf );
		break;
	case kFeedback:
		sprintf( buf, "%2.0f", _feedback * 100.0 );
		strcpy( text,buf );
		break;
	case kStages:
		sprintf( buf, "%d", _stages );
		break;
	case kMixMode:
		switch(_mixMode)
		{
		case kMixMono:
			strcpy( text, "mono" );
			break;
		case kMixMonoMinus:
			strcpy( text, "-mono" );
			break;
		case kMixWetLeft:
			strcpy( text, "wet left" );
			break;
		case kMixWetRight:
			strcpy( text, "wet right" );
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
void APhaser::getParameterLabel (VstInt32 index, char *label)
{
	switch (index)
	{
		case kRate:		strcpy (label, "Hz");		break;
		case kWidth:	strcpy( label, "%");		break;
		case kFeedback:	strcpy( label, "%");		break;
		case kStages:	strcpy( label, "");			break;
		case kMixMode:	strcpy( label, "");			break;
	}
}

//
//								getEffectName
//
bool APhaser::getEffectName (char* name)
{
	strcpy (name, "Phase Shifter");
	return true;
}

//
//								getProductString
//
bool APhaser::getProductString (char* text)
{
	strcpy (text, "Phase Shifter");
	return true;
}

//
//								getVendorString
//
bool APhaser::getVendorString (char* text)
{
	strcpy (text, "DarkVapor");
	return true;
}

//
//								processReplacing
//
void APhaser::processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames)
{
	float* in1 = inputs[0];
	float* in2 = inputs[1];
	float* out1 = outputs[0];
	float* out2 = outputs[1];

	switch( _stages)
	{
	case 2:
		for( int i = 0; i < sampleFrames; i++ )
		{
			double coef = (1.0 - _wp) / (1.0 + _wp);     // calc coef for current freq

			// get input value
			double inval = (in1[i] + in2[i]) / 2.0f;
			double inmix = inval  + _feedback * _feedbackPhase * _ly2;

			// run thru the all pass filters
			_ly1 = coef * (_ly1 + inmix) - _lx1;		// do 1st filter
			_lx1 = inmix;
			_ly2 = coef * (_ly2 + _ly1) - _lx2;			// do 2nd filter
			_lx2 = _ly1;

			// develop output mix
			out1[i] = (float)PIN(_mixLeftDry * inval + _mixLeftWet * _ly2,-0.99,0.99);
			out2[i] = (float)PIN(_mixRightDry * inval + _mixRightWet * _ly2,-0.99,0.99);

			// step the sweep
			_wp *= _sweepFactor;                    // adjust freq of filters
			if(_wp > _maxwp || _wp < _minwp)		// max or min?
			{
				_sweepFactor = 1.0 / _sweepFactor;	// reverse
			}
		}
		break;

	case 4:
	default:
		for( int i = 0; i < sampleFrames; i++ )
		{
			double coef = (1.0 - _wp) / (1.0 + _wp);     // calc coef for current freq

			// get input value
			float inval = (in1[i] + in2[i]) / 2.0f;
			double inmix = inval + _feedback * _feedbackPhase * _ly4;

			// run thru the all pass filters
			_ly1 = coef * (_ly1 + inmix) - _lx1;		// do 1st filter
			_lx1 = inmix;
			_ly2 = coef * (_ly2 + _ly1) - _lx2;			// do 2nd filter
			_lx2 = _ly1;
			_ly3 = coef * (_ly3 + _ly2) - _lx3;			// do 3rd filter
			_lx3 = _ly2;
			_ly4 = coef * (_ly4 + _ly3) - _lx4;			// do 4th filter 
			_lx4 = _ly3;

			// develop output mix
			out1[i] = (float)tanh(_mixLeftDry * inval + _mixLeftWet * _ly2);
			out2[i] = (float)tanh(_mixRightDry * inval + _mixRightWet * _ly2);

			// step the sweep
			_wp *= _sweepFactor;                    // adjust freq of filters
			if(_wp > _maxwp || _wp < _minwp)		// max or min?
			{
				_sweepFactor = 1.0 / _sweepFactor;	// reverse
			}
		}
		break;
	case 6:
		for( int i = 0; i < sampleFrames; i++ )
		{
			double coef = (1.0 - _wp) / (1.0 + _wp);     // calc coef for current freq

			// get input value
			float inval = (in1[i] + in2[i]) / 2.0f;
			double inmix = inval + _feedback * _feedbackPhase * _ly6;

			// run thru the all pass filters
			_ly1 = coef * (_ly1 + inmix) - _lx1;		// do 1st filter
			_lx1 = inmix;
			_ly2 = coef * (_ly2 + _ly1) - _lx2;			// do 2nd filter
			_lx2 = _ly1;
			_ly3 = coef * (_ly3 + _ly2) - _lx3;			// do 3rd filter
			_lx3 = _ly2;
			_ly4 = coef * (_ly4 + _ly3) - _lx4;			// do 4th filter 
			_lx4 = _ly3;
			_ly5 = coef * (_ly5 + _ly4) - _lx5;			// do 5th filter
			_lx5 = _ly4;
			_ly6 = coef * (_ly6 + _ly5) - _lx6;			// do 6th filter 
			_lx6 = _ly5;

			// develop output mix
			out1[i] = (float)tanh(_mixLeftDry * inval + _mixLeftWet * _ly2);
			out2[i] = (float)tanh(_mixRightDry * inval + _mixRightWet * _ly2);

			// step the sweep
			_wp *= _sweepFactor;                    // adjust freq of filters
			if(_wp > _maxwp || _wp < _minwp)		// max or min?
			{
				_sweepFactor = 1.0 / _sweepFactor;	// reverse
			}
		}
		break;

	case 8:
		for( int i = 0; i < sampleFrames; i++ )
		{
			double coef = (1.0 - _wp) / (1.0 + _wp);     // calc coef for current freq

			// get input value
			float inval = (in1[i] + in2[i]) / 2.0f;
			double inmix = inval + _feedback * _feedbackPhase * _ly8;

			// run thru the all pass filters
			_ly1 = coef * (_ly1 + inmix) - _lx1;		// do 1st filter
			_lx1 = inmix;
			_ly2 = coef * (_ly2 + _ly1) - _lx2;			// do 2nd filter
			_lx2 = _ly1;
			_ly3 = coef * (_ly3 + _ly2) - _lx3;			// do 3rd filter
			_lx3 = _ly2;
			_ly4 = coef * (_ly4 + _ly3) - _lx4;			// do 4th filter 
			_lx4 = _ly3;
			_ly5 = coef * (_ly5 + _ly4) - _lx5;			// do 5th filter
			_lx5 = _ly4;
			_ly6 = coef * (_ly6 + _ly5) - _lx6;			// do 6th filter 
			_lx6 = _ly5;
			_ly7 = coef * (_ly7 + _ly6) - _lx7;			// do 7th filter
			_lx7 = _ly6;
			_ly8 = coef * (_ly8 + _ly7) - _lx8;			// do 8th filter 
			_lx8 = _ly7;

			// develop output mix
			out1[i] = (float)tanh(_mixLeftDry * inval + _mixLeftWet * _ly2);
			out2[i] = (float)tanh(_mixRightDry * inval + _mixRightWet * _ly2);

			// step the sweep
			_wp *= _sweepFactor;                    // adjust freq of filters
			if(_wp > _maxwp || _wp < _minwp)		// max or min?
			{
				_sweepFactor = 1.0 / _sweepFactor;	// reverse
			}
		}
		break;
	case 10:
		for( int i = 0; i < sampleFrames; i++ )
		{
			double coef = (1.0 - _wp) / (1.0 + _wp);     // calc coef for current freq

			// get input value
			float inval = (in1[i] + in2[i]) / 2.0f;
			double inmix = inval + _feedback * _feedbackPhase * _ly10;

			// run thru the all pass filters
			_ly1  = coef * (_ly1 + inmix) - _lx1;			// do 1st filter
			_lx1  = inmix;
			_ly2  = coef * (_ly2 + _ly1) - _lx2;			// do 2nd filter
			_lx2  = _ly1;
			_ly3  = coef * (_ly3 + _ly2) - _lx3;			// do 3rd filter
			_lx3  = _ly2;
			_ly4  = coef * (_ly4 + _ly3) - _lx4;			// do 4th filter 
			_lx4  = _ly3;
			_ly5  = coef * (_ly5 + _ly4) - _lx5;			// do 5th filter
			_lx5  = _ly4;
			_ly6  = coef * (_ly6 + _ly5) - _lx6;			// do 6th filter 
			_lx6  = _ly5;
			_ly7  = coef * (_ly7 + _ly6) - _lx7;			// do 7th filter
			_lx7  = _ly6;
			_ly8  = coef * (_ly8 + _ly7) - _lx8;			// do 8th filter 
			_lx8  = _ly7;
			_ly9  = coef * (_ly9 + _ly8) - _lx9;			// do 9th filter
			_lx9  = _ly8;
			_ly10 = coef * (_ly10 + _ly9) - _lx10;			// do 10th filter 
			_lx10 = _ly9;

			// develop output mix
			out1[i] = (float)tanh(_mixLeftDry * inval + _mixLeftWet * _ly2);
			out2[i] = (float)tanh(_mixRightDry * inval + _mixRightWet * _ly2);

			// step the sweep
			_wp *= _sweepFactor;                    // adjust freq of filters
			if(_wp > _maxwp || _wp < _minwp)		// max or min?
			{
				_sweepFactor = 1.0 / _sweepFactor;	// reverse
			}
		}
		break;
	}

}
