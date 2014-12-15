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

#include <stdio.h>
#include <math.h>

#include "../JuceLibraryCode/JuceHeader.h"

class DelayLine
{
public:
    DelayLine();
    DelayLine(float delay = 0, unsigned long maxDelay = 4095);
    ~DelayLine();
    
    /* returns the maximum amount of delay */
    unsigned long getMaxDelay();
    
    /* sets the maximum amount of delay */
    void setMaxDelay(unsigned long delay);
    
    /* sets the delay line delay */
    void setDelay(float delay);
    
    /* returns the delay amount */
    float getDelay();
    
    /* return the value at tapDelay samples from delay line input */
    float tapOut(unsigned long tapDelay);
    
    /* set the value at tapDelay samples from delay line input */
    void tapIn(float value, unsigned long tapDelay);
    
    /* returns next value to be outputted by delay line */
    float nextOut();
    
    /* processes a sample in the delay line */
    float processSample(float inputSample);
    
protected:
    unsigned long _inPoint; //write point
    unsigned long _outPoint; // read point
    float _delay; // delay amount
    float _alpha;
    float _omAlpha;
    float _nextOutput;
    bool _doNextOut; 
    float *_sampBuffer;
    unsigned long _maxDelay;
};



#endif /* defined(__NebuloMod__DelayLine__) */
