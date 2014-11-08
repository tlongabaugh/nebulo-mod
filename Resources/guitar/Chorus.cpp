//
//								Chorus.cpp
//

#include "filter.h"
#include "math.h"

extern void outputDebugDouble(char *msg, double d);

//
//	modf - vaguely related to the library routine modf(), this macro breaks a double into
//	integer and fractional components i and f respectively.
//
//	n - input number, a double
//	i - integer portion, an integer (the input number integer portion should fit)
//	f - fractional portion, a double
//
#define	MODF(n,i,f) ((i) = (int)(n), (f) = (n) - (double)(i))

//
//								Chorus
//
Chorus::Chorus()
{
	for (int i = 0; i < BSZ; i++)
	{
		_buf[i] = 0.0;
	}
	_fp = 0;
	SetSweep(0.05, 0.7);
}

//
//								SetSweep
//
//	Sets up sweep based on rate, depth
//
void Chorus::SetSweep( double rate, double width)
{
	// go with a fixed 28ms delay
	_delaySamples = (int)round(0.028 * SAMPLERATE);
	_sweepSamples = (int)round(width * 0.05 * SAMPLERATE);

	// calc # of samples per second we'll need to move to achieve spec'd sweep rate
	_step = (double)(_sweepSamples * 2.0 * rate) / (double)SAMPLERATE;

	// calc min and max sweep now
	_minSweepSamples = _delaySamples;
	_maxSweepSamples = _delaySamples + _sweepSamples;

	// set intial sweep pointer to midrange
	_sweep = (_minSweepSamples + _maxSweepSamples) / 2;
}

//
//								Process
//
double Chorus::Process(double inval)
{
	// assemble mono input value and store it in circle queue
	_buf[_fp] = inval;
	_fp = (_fp + 1) & (BSZ - 1);

	// build the two emptying pointers and do linear interpolation
	int ep1, ep2;
	double w1, w2;
	double ep = _fp - _sweep;
	MODF(ep, ep1, w2);
	ep1 &= (BSZ - 1);
	ep2 = ep1 + 1;
	ep2 &= (BSZ - 1);
	w1 = 1.0 - w2;
	double outval = _buf[ep1] * w1 + _buf[ep2] * w2;

	// increment the sweep
	_sweep += _step;
	if (_sweep >= _maxSweepSamples || _sweep <= _minSweepSamples)
	{
		_step = -_step;
	}
	return outval;
}
