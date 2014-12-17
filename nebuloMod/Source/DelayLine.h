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
    virtual void setDelay(double delay);
    
    /* returns the delay amount */
    double getDelay();
    
    /* returns next value to be outputted by delay line */
    virtual float nextOut();
    
    /* processes a sample in the delay line */
    virtual float processSample(float inputSample);
    
    // clears the delay buffer
    void clear();
    
protected:
    unsigned long _inPoint; //write point
    unsigned long _outPoint; // read point
    double _delay; // delay amount
    double _alpha;
    double _omAlpha;
    double _nextOutput;
    bool _doNextOut; 
    float *_sampBuffer;
    unsigned long _maxDelay;
};



#endif /* defined(__NebuloMod__DelayLine__) */
