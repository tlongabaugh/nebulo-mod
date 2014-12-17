//
//  DelayLine.h
//  NebuloMod
//
//  Created by Tom Longabaugh on 12/13/14.
//
//  with help from the STK github.com/thestk/stk/blob/master/include/DelayL.h and
//  github.com/thestk/stk/blob/master/include/DelayL.cpp

#ifndef __NebuloMod__DelayLine__
#define __NebuloMod__DelayLine__

#include "../JuceLibraryCode/JuceHeader.h"

class DelayLine
{
public:
    DelayLine(double delay = 1.0, unsigned long maxDelay = 4096);
    virtual ~DelayLine();
    
    /* returns the maximum amount of delay */
    unsigned long getMaxDelay();
    
    /* sets the maximum amount of delay */
    void setMaxDelay(unsigned long delay);
    
    /* sets the delay line delay */
    void setDelay(double delay);
    
    /* returns the delay amount */
    double getDelay();
    
    /* returns next value to be outputted by delay line */
    float nextOut();
    
    /* processes a sample in the delay line */
    float processSample(float inputSample);
    
    // clears the delay buffer
    void clear();
    
protected:
    unsigned long _inPoint;     //write point
    unsigned long _outPoint;    // read point
    double _delay;              // delay amount
    double _alpha;              // fractional value
    double _omAlpha;            // fractional value
    double _nextOutput;         // next output from table
    bool _doNextOut;            // checks to see if we need to calc next out
    float *_sampBuffer;         // the delay line buffer
    unsigned long _maxDelay;    // The max amount of delay (in samples)
};

#endif /* defined(__NebuloMod__DelayLine__) */


