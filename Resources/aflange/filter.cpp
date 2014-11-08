//
//								filter.cpp
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

//
//								TwoPoleLowPassFilter
//
TwoPoleLowPassFilter::TwoPoleLowPassFilter( float cutoff)
{
	double Q = 1.1f;
    w0 = 2 * M_PI * cutoff / SAMPLE_RATE;
    alpha = sin(w0) / (2.0 * Q);
    b0 =  (1.0 - cos(w0))/2;
    b1 =   1.0 - cos(w0);
    b2 =  (1.0 - cos(w0))/2;
    a0 =   1.0 + alpha;
    a1 =  -2.0 * cos(w0);
    a2 = 1.0 - alpha;
	x1 = x2 = y1 = y2 = 0;

	// DEBUG
	char buf[256];
	sprintf( buf, "LowPass: a0=%f a1=%f a2=%f b0=%f b1=%f b2=%f\n", a0, a1, a2, b0, b1, b2);
	OutputDebugString(buf);
}

//
//								Process
//
double TwoPoleLowPassFilter::Process(double x)
{
    double y = (b0/a0)*x + (b1/a0)*x1 + (b2/a0)*x2 - (a1/a0)*y1 - (a2/a0)*y2;                  
    x2 = x1;
    x1 = x;
    y2 = y1;
    y1 = y;
    return y;
}
