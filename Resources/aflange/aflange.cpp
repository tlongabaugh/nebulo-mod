//
//								aflange.cpp
//

#include <stdio.h>
#include <string.h>
#include <math.h>

#ifndef __aflange__
#include "aflange.h"
#endif

#ifndef __sdeditor__
#include "editor/sdeditor.h"
#endif

//#define EFDEBUG

//
//								debugOut
//
void debugOut( char *str, int p)
{
#ifdef EFDEBUG
	char buf[256];
	sprintf( buf, str, p );
	OutputDebugString(buf);
#endif
}

//
//								debugOutD
//
void debugOutD( char *str, double n)
{
#ifdef EFDEBUG
	char buf[256];
	sprintf( buf, str, n );
	OutputDebugString(buf);
#endif
}

//
//								debugOut2D
//
void debugOut2D( char *str, double d1, double d2)
{
#ifdef EFDEBUG
	char buf[256];
	sprintf( buf, str, d1, d2);
	OutputDebugString(buf);
#endif
}

//
//								AFlangeProgram 
//
AFlangeProgram::AFlangeProgram ()
{
	// default Program Values
	paramSweepRate = 0.1f;
	paramWidth = 0.3f;
	paramFeedback = 0.0f;
	paramDelay = 0.2f;
	paramMix = 1.0f;
	paramMixMode = 0.0f;
	strcpy (paramName, "Init");
}

//
//								AFlangeProgram
//
AFlangeProgram::AFlangeProgram( float sweep, float width, float feedback, float delay, float mix, float mixMode, char *name )
{
	paramSweepRate = sweep;
	paramWidth = width;
	paramFeedback = feedback;
	paramDelay = delay;
	paramMix = mix;
	paramMixMode = mixMode;
	strncpy( paramName, name, kMaxNameLength-1);
	paramName[kMaxNameLength - 1] = 0;
}

//
//								AFlange 
//
AFlange::AFlange(audioMasterCallback audioMaster)
	: AudioEffectX (audioMaster, kNumPrograms, kNumParams)
{
	debugOut( "\n\nAFlange 0.5\n\n", 0 );

	// init
	pPrograms[0] = new AFlangeProgram( 0.1f, 0.3f, 0.0f, 0.1f, 1.0f, 0.0f, "Slow Mono");
	pPrograms[1] = new AFlangeProgram( 0.3f, 0.3f, 0.0f, 0.1f, 1.0f, 0.0f, "Medium Mono");
	pPrograms[2] = new AFlangeProgram( 0.5f, 0.3f, 0.0f, 0.1f, 1.0f, 0.0f, "Fast Mono");

	pPrograms[3] = new AFlangeProgram( 0.1f, 0.3f, 0.0f, 0.1f, 1.0f, (float)kMixMonoMinus / (float)kNumMixModes, "Slow Mono Minus");
	pPrograms[4] = new AFlangeProgram( 0.3f, 0.3f, 0.0f, 0.1f, 1.0f, (float)kMixMonoMinus / (float)kNumMixModes, "Medium Mono Minus");
	pPrograms[5] = new AFlangeProgram( 0.5f, 0.3f, 0.0f, 0.1f, 1.0f, (float)kMixMonoMinus / (float)kNumMixModes, "Fast Mono Minus");

	pPrograms[6] = new AFlangeProgram( 0.1f, 0.3f, 0.0f, 0.1f, 1.0f, (float)kMixStereo / (float)kNumMixModes, "Slow Stereo");
	pPrograms[7] = new AFlangeProgram( 0.3f, 0.3f, 0.0f, 0.1f, 1.0f, (float)kMixStereo / (float)kNumMixModes, "Medium Stereo");
	pPrograms[8] = new AFlangeProgram( 0.5f, 0.3f, 0.0f, 0.1f, 1.0f, (float)kMixStereo / (float)kNumMixModes, "Fast stereo");

	pPrograms[9] = new AFlangeProgram( 0.0f, 0.0f, 0.0f, 0.1f, 1.0f, (float)kMixStereo / (float)kNumMixModes, "Automation Stereo");
	pPrograms[10] =new AFlangeProgram( 0.0f, 0.0f, 0.0f, 0.1f, 1.0f, (float)kMixStereoMinus / (float)kNumMixModes, "Automation Stereo Minus");

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
	setSweep();
	_mixMode = 0;
	_fp = 0;
	_sweep = 0.0;
	_outval1 = _outval2 = 0.0f;
	_mixLeftWet =
	_mixLeftDry =
	_mixRightWet =
	_mixRightDry = 0.5f;
	_mixMono = true;

	// prep control filtering
	_delayControlFilter = new TwoPoleLowPassFilter( 20.0f );
	_mixControlFilter = new TwoPoleLowPassFilter( 20.0f );

	// set initial program (defaults)
	if (pPrograms[0] != NULL)
	{
		setProgram (0);
	}

	setNumInputs (2);	// pseudo-mono input
	setNumOutputs (2);	// pseudo-mono output

	setUniqueID ('mm91');	// this should be unique, use the Steinberg web page for plugin Id registration

	// create the editor
	editor = new SDEditor (this);

	// allocate the buffers
	_buf1 = new double[BSZ];
	_buf2 = new double[BSZ];
	for( int i = 0; i < BSZ; i++)
	{
		_buf1[i] = _buf2[i] = 0.0;
	}

	resume ();		// flush buffer
}

//
//								~AFlange 
//
AFlange::~AFlange ()
{
	debugOut("~AFlange\n",0);

	for( int i = 0; i < kNumPrograms; i++)
	{
		AFlangeProgram *p = pPrograms[i];
		delete p;
		pPrograms[i] = NULL;
	}

	delete _delayControlFilter;
	delete _mixControlFilter;

	if( _buf1 )
		delete[] _buf1;
	if( _buf2 )
		delete[] _buf2;
}

//
//								setProgram
//
void AFlange::setProgram (VstInt32 program)
{
	debugOut( "setProgram(%d)\n", program);
	AFlangeProgram* ap = pPrograms[program];

	curProgram = program;
	setParameter (kRate, ap->paramSweepRate);	
	setParameter (kWidth, ap->paramWidth);
	setParameter (kFeedback, ap->paramFeedback);
	setParameter (kDelay, ap->paramDelay);
	setParameter (kMix, ap->paramMix);
	setParameter (kMixMode, ap->paramMixMode);

	// the VST host needs to force a repaint now
	updateDisplay();
}

//
//								setRate
//
void AFlange::setRate (float rate)
{
	debugOutD( "setRate(%f)\n", rate );

	_paramSweepRate = rate;
	pPrograms[curProgram]->paramSweepRate = rate;

	// map into param onto 0.05Hz - 10hz with log curve
	_sweepRate = pow(10.0,(double)_paramSweepRate);
	_sweepRate  -= 1.0;
	_sweepRate  *= 1.05556f;
	_sweepRate  += 0.05f;

	// finish setup
	setSweep();
}


//
//								setWidth
//
//	Maps 0.0-1.0 input to calculated width in samples from 0ms to 10ms
//
void AFlange::setWidth (float v)
{
	debugOutD( "setWidth(%f)\n", v );

	_paramWidth = v;
	pPrograms[curProgram]->paramWidth = v;

	// map so that we can spec between 0ms and 10ms
	if( v <= 0.05)
	{
		// eat some noise on the bottom end
		_sweepSamples = 0.0;
	}
	else
	{
		// otherwise calc # of samples for the total width
		_sweepSamples = v * 0.01 * SAMPLE_RATE;
	}

	// finish setup
	setSweep();
}

//
//								setDelay
//
void AFlange::setDelay (float v)
{
	debugOutD( "setDelay(%f)\n", v );
	_paramDelay = v;
	pPrograms[curProgram]->paramDelay = v;
}

//
//								setSweep
//
//	Sets up sweep based on rate, depth, and delay as they're all interrelated
//	Assumes _sweepRate and _sweepSamples have all been set by
//	setRate and setWidth and calcs the steps to achieve the desired sweep rate
//
void AFlange::setSweep()
{
	debugOut("setSweep()\n",0);

	// calc # of samples per second we'll need to move to achieve spec'd sweep rate
	_step = (double)(_sweepSamples * 2.0 * _sweepRate) / (double)SAMPLE_RATE;

	// calc max and start sweep at 0.0
	_sweep = 0.0;
	_maxSweepSamples = _sweepSamples;
	debugOutD( "  _max=%f\n", _maxSweepSamples);
}


//
//								setFeedback
//
void AFlange::setFeedback(float v)
{
	debugOutD("setFeedback(%f)\n",v );
	_paramFeedback = v;
	pPrograms[curProgram]->paramFeedback = v;
	_feedback = v;
}

//
//								setMix
//
void AFlange::setMix (float v)
{
	debugOutD("setMix(%f)\n",v);

	_paramMix = v;
	pPrograms[curProgram]->paramMix = v;
}

//
//								setMixMode
//
//	Converts the floating point value to an index list of the supported mix modes.
//
void AFlange::setMixMode (float v)
{
	debugOutD("setMixMode(%f)\n",v);

	_paramMixMode = v;
	pPrograms[curProgram]->paramMixMode = v;
	_mixMode = ROUND(v * kNumMixModes);
	debugOutD("_mixMode=%f\n", (double)_mixMode);

	switch(_mixMode)
 	{
	case kMixMono:
	default:
		_mixMono = true;
		_mixLeftWet = _mixRightWet = _mixLeftDry = _mixRightDry = 1.0;
		_feedbackPhase = 1.0;
		break;
	case kMixMonoMinus:
		_mixMono = true;
		_mixLeftWet = _mixRightWet = -1.0;
		_mixLeftDry = _mixRightDry = 1.0;
		_feedbackPhase = -1.0;
		break;
	case kMixMonoBoth:
		_mixMono = true;
		_mixLeftWet = _mixLeftDry = _mixRightDry = 1.0;
		_mixRightWet = -1.0;
		_feedbackPhase = 1.0;
		break;
	case kMixStereo:
		_mixMono = false;
		_mixLeftWet = _mixRightWet = 1.0;
		_mixLeftDry = _mixRightDry = 1.0;
		_feedbackPhase = 1.0;
		break;
	case kMixStereoMinus:
		_mixMono = false;
		_mixLeftWet = _mixRightWet = -1.0;
		_mixLeftDry = _mixRightDry = 1.0;
		_feedbackPhase = -1.0;
		break;
	case kMixStereoBoth:
		_mixMono = false;
		_mixLeftWet = _mixLeftDry = _mixRightDry = 1.0;
		_mixRightWet = -1.0;
		_feedbackPhase = 1.0;
		break;
	}
}



//
//								setProgramName
//
void AFlange::setProgramName (char *name)
{
	debugOut("setProgramName\n",0);
	strcpy (pPrograms[curProgram]->paramName, name);
}

//
//								getProgramName
//
void AFlange::getProgramName (char *name)
{
	debugOut("getProgramName\n",0);
	if (!strcmp (pPrograms[curProgram]->paramName, "Init"))
		sprintf (name, "%s %d", pPrograms[curProgram]->paramName, curProgram + 1);
	else
		strcpy (name, pPrograms[curProgram]->paramName);
}

//
//								getProgramNameIndexed
//
bool AFlange::getProgramNameIndexed (VstInt32 category, VstInt32 index, char* text)
{
	debugOut("getProgramNameIndexed index=%d\n", index);
	if (index < kNumPrograms)
	{
		strcpy (text, pPrograms[index]->paramName);
		return true;
	}
	return false;
}

//
//								resume
//
void AFlange::resume ()
{
	debugOut("resume\n",0);
	AudioEffectX::resume ();
}

//
//								setParameter
//
void AFlange::setParameter (VstInt32 index, float value)
{
	debugOut2D("setParameter(%f,%f)\n", index, value);

	switch (index)
	{
		case kRate:    setRate(value);					break;
		case kWidth:   setWidth(value);					break;
		case kFeedback: setFeedback(value);				break;
		case kDelay:	setDelay(value);				break;
		case kMix:		setMix(value);					break;
		case kMixMode: setMixMode(value);				break;
	}
}

//
//								getParameter
//
float AFlange::getParameter (VstInt32 index)
{
	debugOut("getParameter(%d)\n",index);
	float v = 0;

	switch (index)
	{
		case kRate :    v = _paramSweepRate;	break;
		case kWidth:	v = _paramWidth;		break;
		case kFeedback: v = _paramFeedback;		break;
		case kDelay:	v = _paramDelay;		break;
		case kMix:		v = _paramMix;			break;
		case kMixMode:	v = _paramMixMode;		break;
	}
	return v;
}

//
//								getParameterName
//
void AFlange::getParameterName (VstInt32 index, char *label)
{
	debugOut("getParameterName index=%d\n", index);
	switch (index)
	{
		case kRate:    strcpy (label, "Rate");			break;
		case kWidth:   strcpy (label, "Width");			break;
		case kFeedback: strcpy(label, "Feedback");		break;
		case kDelay:	strcpy (label, "Delay");		break;
		case kMix:		strcpy( label, "Mix");			break;
		case kMixMode: strcpy (label, "Mix Mode");		break;
	}
}

//
//								getParameterDisplay
//
void AFlange::getParameterDisplay (VstInt32 index, char *text)
{
	debugOut("getParameterDisplay index=%d\n", index);
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
	case kMix:
		sprintf( buf, "%2.0f", _paramMix );
		break;
	case kMixMode:
		switch(_mixMode)
		{
		case kMixMono:
			strcpy( text, "mono" );
			break;
		case kMixMonoMinus:
			strcpy( text, "mono-" );
			break;
		case kMixMonoBoth:
			strcpy( text, "mono+/-" );
		case kMixStereo:
			strcpy( text, "stereo" );
			break;
		case kMixStereoMinus:
			strcpy( text, "stereo-" );
			break;
		case kMixStereoBoth:
			strcpy( text, "stereo+/-" );
			break;
		}
		break;
	}
}

//
//								getParameterLabel
//
void AFlange::getParameterLabel (VstInt32 index, char *label)
{
	debugOut("getParameterLabel index=%d\n", index);
	switch (index)
	{
		case kRate:		strcpy (label, "Hz");		break;
		case kWidth:	strcpy( label, "%");		break;
		case kFeedback:	strcpy( label, "%");		break;
		case kDelay:	strcpy( label, "");			break;
		case kMix:		strcpy( label, "%");		break;
		case kMixMode:	strcpy( label, "");			break;
	}
}

//
//								getEffectName
//
bool AFlange::getEffectName (char* name)
{
	debugOut("getEffectName\n",0);
	strcpy (name, "Auto-Flange");
	return true;
}

//
//								getProductString
//
bool AFlange::getProductString (char* text)
{
	debugOut("getProductString\n",0);
	strcpy (text, "Auto-Flange");
	return true;
}

//
//								getVendorString
//
bool AFlange::getVendorString (char* text)
{
	debugOut("getVendorString\n",0);
	strcpy (text, "ChewingAluminumFoil");
	return true;
}

#ifdef DEBUG_PROCESSING
int debugSampleCount = 0;
#endif

//
//								fpin
//
//	Pins within the -1 to 1 range
double fpin( double n)
{
	if( n < -0.999)
	{
		return -0.999;
	}
	else if( n > 0.999)
	{
		return 0.999;
	}
	else
	{
		return n;
	}
}


//
//								processReplacing
//
//	Main worker routine, does the audio processing for the selected flanging
//
void AFlange::processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames)
{
	float* in1 = inputs[0];
	float* in2 = inputs[1];
	float* out1 = outputs[0];
	float* out2 = outputs[1];

	for( int i = 0; i < sampleFrames; i++ )
	{
		// see if we're doing mono
		float inval1, inval2;
		if( _mixMono )
		{
			inval1 = inval2 = (in1[i] + in2[i]) / 2.0f;
		}
		else
		{
			inval1 = in1[i];
			inval2 = in2[i];
		}
		double inmix1 = inval1 + _feedback * _feedbackPhase * _outval1;
		double inmix2 = inval2 + _feedback * _feedbackPhase * _outval2;

		_buf1[_fp] = inmix1;
		_buf2[_fp] = inmix2;
		_fp = (_fp + 1) & (BSZ-1);

		// get filtered delay
		double delay = _delayControlFilter->Process( _paramDelay);
#ifdef EFDEBUG
		debugOut2D( "  param: %f delay: %f\n", _paramDelay, delay);
#endif

		// delay 0.0-1.0 maps to 0.02ms to 10ms (always have at least 1 sample of delay)
		double delaySamples = (delay * SAMPLE_RATE * 0.01) + 1.0;
		delaySamples += _sweep;
#ifdef EFDEBUG
		debugOutD( "  delaySamples: %f\n", delaySamples);
#endif

		// build the two emptying pointers and do linear interpolation
		int ep1, ep2;
		double w1, w2;
		double ep = _fp - delaySamples;
		if( ep < 0.0)
		{
			ep += BSZ;
		}
		MODF(ep, ep1, w2);
		ep1 &= (BSZ-1);
		ep2 = ep1 + 1;
		ep2 &= (BSZ-1);
		w1 = 1.0 - w2;
		_outval1 = _buf1[ep1] * w1 + _buf1[ep2] * w2;
		_outval2 = _buf2[ep1] * w1 + _buf2[ep2] * w2;

		// get filtered mix
		double mix = _mixControlFilter->Process( _paramMix);

		// develop output mix
		double f1 = _mixLeftDry * inval1 + _mixLeftWet * mix * _outval1; 
		double f2 = _mixRightDry * inval2 + _mixRightWet * mix * _outval2;
		f1 = fpin( f1 );
		f2 = fpin( f2 );

		// pin to desired output range
		out1[i] = (float)PIN( f1, -0.99, 0.99 );
		out2[i] = (float)PIN( f2, -0.99, 0.99 );

		// see if we're doing sweep
		if( _step != 0.0)
		{
			// increment the sweep
			_sweep += _step;
			if( _sweep <= 0.0)
			{
				// make sure we don't go negative
				_sweep = 0.0;
				// and reverse direction
				_step = -_step;
			}
			else if( _sweep >=  _maxSweepSamples)
			{
				_step = -_step;
			}
		}
	}

}
