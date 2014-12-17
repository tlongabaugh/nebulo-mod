//
//  DelayLine.cpp
//  NebuloMod
//
//  Created by Tom Longabaugh on 12/13/14.
//
//

#include "DelayLine.h"

DelayLine::DelayLine(double delay, unsigned long maxDelay)
{
    _maxDelay = maxDelay;
    // Raise assert if expression is not true
    jassert(delay > 0.0);
    jassert(delay <= (float)maxDelay);
    
    // declare the sample buffer...make sure to delete!
    _sampBuffer = new float[maxDelay];
    memset((void*)_sampBuffer, 0, sizeof(float)*maxDelay);
    
    _inPoint = 0;
    this->setDelay(delay);
    _doNextOut = true;

    
}

DelayLine::~DelayLine()
{
    if(_sampBuffer){
        delete [] _sampBuffer;
    }
}

unsigned long DelayLine::getMaxDelay()
{
    return _maxDelay;
}

void DelayLine::setMaxDelay(unsigned long delay)
{
    // this is probably really bad practice.....
    delete [] _sampBuffer;
    _sampBuffer = new float[delay];
    memset((void*)_sampBuffer, 0, sizeof(float)*delay);
    _maxDelay = delay;
}


void DelayLine::setDelay(double delay)
{
    // make sure the delay is valid
    jassert(delay < (float)_maxDelay);
    jassert(delay >= 0.0);
    
    // read follows write
    double outPointer = _inPoint - delay;
    _delay = delay;
    
    // wrap pointer
    while (outPointer < 0) {
        outPointer += (double)_maxDelay;
    }
    
    // integer part of delay
    _outPoint = (long)outPointer;
    
    // fractional part of delay
    _alpha = outPointer - _outPoint;
    _omAlpha = (double)1.0 - _alpha;
    
    // wrap the buffer
    if (_outPoint == _maxDelay) {
        _outPoint = 0;
    }
    
    _doNextOut = true;
}

double DelayLine::getDelay()
{
    return _delay;
}

float DelayLine::nextOut()
{
    // if delay is greater than 0
    if (_doNextOut) {
        // first part of interpolation
        _nextOutput = _sampBuffer[_outPoint] * _omAlpha;
        // second part of interpolation (fractional)
        if (_outPoint + 1 < _maxDelay) {
            _nextOutput += _sampBuffer[_outPoint+1] * _alpha;
        }
        else {
            _nextOutput += _sampBuffer[0] * _alpha;
        }
        _doNextOut = false;
    }
    return _nextOutput;
}

float DelayLine::processSample(float inputSample)
{
    float output;
    
    // copy input sample
    _sampBuffer[_inPoint++] = inputSample;
    
    // wrap input pointer if necessary
    if (_inPoint == _maxDelay) {
        _inPoint = 0;
    }
    
    // get output value, set to find output value again
    output = nextOut();
    _doNextOut = true;
    
    // wrap outpointer if necessary
    if (++_outPoint == _maxDelay) {
        _outPoint = 0;
    }
    
    return output;
}

void DelayLine::clear()
{
    if (_sampBuffer) {
        memset((void*)_sampBuffer, 0, sizeof(float)*_maxDelay);
    }
}




