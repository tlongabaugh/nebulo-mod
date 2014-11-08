//
//								Limiter.cpp
//

#include "filter.h"
#include "math.h"

extern void outputDebugDouble(char *msg, double d);

//
//								Limiter
//
Limiter::Limiter(double releaseMs)
{
	// and release
	_release = pow(0.01, 1.0 / (releaseMs * 0.001 * SAMPLERATE));
	for (int i = 0; i < CBSZ; i++)
	{
		_inq[i] = 0;
	}
	SetThreshold(0.2);
}

//
//								SetThreshold
//
void Limiter::SetThreshold(double thresh)
{
	_threshold = thresh;
	outputDebugDouble("thresh: ", thresh);
}

//
//								Limiter::Process
//
double Limiter::Process(double val)
{
	// do RMS
	double oldInput2 = _inq[_fp];
	double val2 = val * val;
	_inq[_fp] = (float)val2;
	_fp = (_fp + 1) % CBSZ;
	_total += val2;
	_total -= oldInput2;
	double rms = sqrt(_total / CBSZ);

	// build that fast attack slow release envelope
	if (rms > _envelope)
	{
		_envelope = rms;
	}
	else
	{
		_envelope *= _release;
	}

	// do adaptive gain
	double gain = 1.0;
	if (_envelope > _threshold)
	{
		gain = _threshold / _envelope;
	}
	else if (_envelope < 0.05)
	{
		gain = _envelope / 0.05;
	}
	return val * gain;
}

