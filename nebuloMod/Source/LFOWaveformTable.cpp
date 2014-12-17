//
//  LFOWaveformTable.cpp
//  NebuloMod
//
//  Created by Ryan Foo on 11/23/14.
//
//

/*
 * GUI ASPECT of the LFO Waveform
 * 1) Allows the user to draw a customizable waveform 
 * 2) Only 2 movable positions and 1 steady position
 * 3) Every time you move a position, store the sample into a buffer
 *
 * Make table buffer public so we can use it in other classes
 */

#include "LFOWaveformTable.h"

LFOWaveformTable::LFOWaveformTable()
{
    // Set the max delay of the buffer, and the inpoint to 0 (inPoint never changes)
    _maxDelay = TABLE_SIZE;
    _inPoint = 0;
    _outPoint = 1;
    
    setSampleRate(INIT_SAMPLE_RATE);
}

LFOWaveformTable::~LFOWaveformTable()
{
}

void LFOWaveformTable::prepareToPlay()
{
    // set frequency and wave form initially
    frequency = 0.5;
    waveForm = sineWave;
}

void LFOWaveformTable::setSampleRate(double sampleRate)
{
    // Set sample rate and coefficients for lowpass
    currentSampleRate = sampleRate;
    lowpass.setCoefficients(coeffs.makeLowPass(currentSampleRate, 900));
}

float LFOWaveformTable::tableLookup()
{
    float output;
    
    // get output value, set to find output value again
    output = nextOut();
    _doNextOut = true;
    
    // wrap outpointer if necessary
    if (++_outPoint == _maxDelay) {
        _outPoint = 0;
    }
    
    return output;
}

float LFOWaveformTable::nextOut()
{
    // if delay is greater than 0 (always will be for this class)
    if (_doNextOut) {
        // first part of interpolation
        _nextOutput = waveformTable[_outPoint] * _omAlpha;
        
        // second part of interpolation (fractional)
        if (_outPoint + 1 < _maxDelay) {
            _nextOutput += waveformTable[_outPoint+1] * _alpha;
        }
        else {
            _nextOutput += waveformTable[0] * _alpha;
        }
        
        _doNextOut = false;
    }
    
    return _nextOutput;
}

void LFOWaveformTable::setIncrement(double increment)
{
    // make sure the delay is valid
    jassert(increment <= (float)_maxDelay);
    jassert(increment >= 0.0);
    
    // read pointer will move through buffer
    double outPointer = _inPoint + increment;
    _delay = increment;
    
    // integer part of delay (rounds outPointer to int value)
    _outPoint = (long)outPointer;
    
    // fractional part of delay
    _alpha = outPointer - _outPoint;
    _omAlpha = (double)1.0 - _alpha;
    
    // wrap the pointer when it gets to end of buffer
    if (_outPoint >= _maxDelay) {
        _outPoint = 0;
    }
    
    _doNextOut = true;
}


float LFOWaveformTable::generateWaveSample()
{
    float waveSample;
    
    // Generate the next wave sample based on GUI input
    if (waveForm == sineWave) {
        waveSample = generateSine(frequency);
    }
    else if (waveForm == triWave) {
        waveSample = generateTriangle(frequency);
    }
    else if (waveForm == sawWave) {
        waveSample = generateSawtooth(frequency);
    }
    else if (waveForm == squareWave) {
        waveSample = generateSquare(frequency);
    }
    else { // lfoTable
        waveSample = generateLFOTableSample(frequency);
    }
    
    return waveSample;
}

float LFOWaveformTable::generateSine(float freq)
{
    float phase, sinSample = 0;
    static float prev_phase;
    
    // Get phase, then get sample
    phase = 2 * M_PI * freq / currentSampleRate + prev_phase;
    sinSample = sin(phase);
    
    // wrap phase
    if (phase > (2 * M_PI))
    {
        phase -= (2 * M_PI);
    }
    
    prev_phase = phase;
    
    return sinSample;
}

float LFOWaveformTable::generateTriangle(float freq)
{
    // Generate triangle wave by absolute valuing saw
    static float triSample;
    static float sawSample2;
    
    float T = currentSampleRate / freq;
    
    // increment, calculate tri sample
    sawSample2 += 2./T;
    triSample = fabsf(sawSample2) * 2. - 1.;
    
    //wrap sample
    if (sawSample2 >= 1)
    {
        sawSample2 -= 2;
    }
    
    return triSample;
}

float LFOWaveformTable::generateSawtooth(float freq)
{
    static float sawSample;
    float T = currentSampleRate / freq;

    // generate saw sample
    sawSample += 2./T;
    
    // decrement to begin period again
    if (sawSample >= 1)
    {
        sawSample -= 2;
    }
    
    // Low pass the saw sample for smoothing
    sawSample = lowpass.processSingleSampleRaw(sawSample);
    
    return sawSample;
    
}

float LFOWaveformTable::generateSquare(float freq)
{
    // Generates a smoothed square by rounding the 1 to -1 to 1 jumps
    float phase, squareSample = 0;
    static float prev_phase;
    
    // Generate a sine sample
    phase = 2 * M_PI * freq / currentSampleRate + prev_phase;
    squareSample = sin(phase);
    
    // Cut the top of it off
    if(squareSample >= 0.2){
        squareSample = 0.2;
    }
    else if (squareSample <= -0.2){
        squareSample = -0.2;
    }
    
    // Get back into correct amplitude
    squareSample *= 5;
    
    // wrap phase
    if (phase > (2 * M_PI))
    {
        phase -= (2 * M_PI);
    }
    
    prev_phase = phase;
    
    return squareSample;
}


float LFOWaveformTable::generateLFOTableSample(float freq)
{
    float tableSample;
        static float tableIncr = 0.0;
    
    // Number of samples per period
    float T = currentSampleRate / freq;
    
    // increment the table value
    tableIncr += (double)(_maxDelay / T);
    
    // wrap the table increment
    if (tableIncr >= 1024.0) {
        tableIncr = 0.0;
    }
    
    // set the read pointer back the correct amount
    setIncrement(tableIncr);
    
    // get the table sample, lowpass it for smoothing
    tableSample = tableLookup();
    
    tableSample = lowpass.processSingleSampleRaw(tableSample);
    
    return tableSample*.9; // make sure we don't out of bounds the delay time
}






