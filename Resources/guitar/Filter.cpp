//
//								Filter.cpp
//
//  Filter design based on info at:
//  http://www.musicdsp.org/files/Audio-EQ-Cookbook.txt
//	and for single-pole:
//	http://www.earlevel.com/main/2012/12/15/a-one-pole-filter/
//

#include "filter.h"
#include "math.h"

#include <stdio.h>

//
//								OnePoleFilter
//
OnePoleFilter::OnePoleFilter(double cutoff)
{
	_fc = cutoff / SAMPLERATE;
}

//
//								TwoPoleFilter
//
TwoPoleFilter::TwoPoleFilter(double cutoff)
{
	_cutoff = cutoff;
	w0 = 2 * PI * _cutoff / SAMPLERATE;
	alpha = sin(w0) / (2.0 * Q);
}



//
//								TwoPoleHighPassFilter
//
TwoPoleHighPassFilter::TwoPoleHighPassFilter(double cutoff) : TwoPoleFilter(cutoff)
{
	b0 = (1.0 + cos(w0)) / 2.0;
	b1 = -(1.0 + cos(w0));
	b2 = (1.0 + cos(w0)) / 2.0;
	a0 = 1.0 + alpha;
	a1 = -2.0 * cos(w0);
	a2 = 1.0 - alpha;
}


//
//								TwoPoleLowPassFilter
//
TwoPoleLowPassFilter::TwoPoleLowPassFilter(double cutoff) : TwoPoleFilter(cutoff)
{
	b0 = (1.0 - cos(w0)) / 2;
	b1 = 1.0 - cos(w0);
	b2 = (1.0 - cos(w0)) / 2;
	a0 = 1.0 + alpha;
	a1 = -2.0 * cos(w0);
	a2 = 1.0 - alpha;
}


//
//								OnePoleHighPassFilter
//
OnePoleHighPassFilter::OnePoleHighPassFilter(double cutoff) : OnePoleFilter(cutoff)
{
	_fc - cutoff / SAMPLERATE;
	double  x = pow(E, -2.0 * PI * _fc);

	_a0 = (1.0 + x) / 2.0;
	_a1 = -(1.0 + x) / 2.0;
	_b1 = x;
	_x1 = _y1 = 0;
}


//
//								OnePoleLowPassFilter
//
OnePoleLowPassFilter::OnePoleLowPassFilter(double cutoff) : OnePoleFilter(cutoff)
{
	_fc - cutoff / SAMPLERATE;
	double  x = pow(E, -2.0 * PI * _fc);

	_a0 = 1.0 - x;
	_b1 = x;
	_a1 = 0.0;
	_x1 = _y1 = 0.0;

}


