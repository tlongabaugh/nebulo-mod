//
//									Phaser.cpp
//

#include "filter.h"
#include "math.h"

#define BOTTOM_FREQ 100

extern void outputDebugDouble(char *msg, double d);

//
//								Phaser::Phaser
//
Phaser::Phaser()
{
	SetSweep(0.3, 0.8);
}

//
//								SetSweep
//
void Phaser::SetSweep(double rateParam, double widthParam)
{
	// calc the actual sweep rate
	double rate = pow(10.0, (double)rateParam);
	rate -= 1.0;
	rate *= 1.1f;
	rate += 0.1f;
	_sweepRate = rate;
	double range = widthParam * 6.0f;

	// do the rest of the inits here just for fun
	_wp = _minwp = (PI * BOTTOM_FREQ) / (double)SAMPLERATE;
	_maxwp = (PI * BOTTOM_FREQ * range) / (double)SAMPLERATE;

	// figure out increment to multiply by each time
	_sweepFactor = pow((double)range, (double)(_sweepRate / (SAMPLERATE / 2.0)));

	// init the all pass line
	_lx1 = _ly1 =
	_lx2 = _ly2 =
	_lx3 = _ly3 =
	_lx4 = _ly4 = 0.0f;
}

//
//								Process
//
double Phaser::Process(double inval)
{
	double coef = (1.0 - _wp) / (1.0 + _wp);     // calc coef for current freq

	// run thru the all pass filters
	_ly1 = coef * (_ly1 + inval) - _lx1;		// do 1st filter
	_lx1 = inval;
	_ly2 = coef * (_ly2 + _ly1) - _lx2;			// do 2nd filter
	_lx2 = _ly1;
	_ly3 = coef * (_ly3 + _ly2) - _lx3;			// do 3rd filter
	_lx3 = _ly2;
	_ly4 = coef * (_ly4 + _ly3) - _lx4;			// do 4th filter 
	_lx4 = _ly3;

	// step the sweep
	_wp *= _sweepFactor;                    // adjust freq of filters
	if (_wp > _maxwp || _wp < _minwp)		// max or min?
	{
		_sweepFactor = 1.0 / _sweepFactor;	// reverse
	}

	// return the output of the all pass chain
	return _ly4;
}