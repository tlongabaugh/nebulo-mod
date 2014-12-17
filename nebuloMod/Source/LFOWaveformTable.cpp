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
    _maxDelay = TABLE_SIZE;
    _inPoint = 0;
    _outPoint = 1;
    
    fillLFOTable(waveformTable);
}

LFOWaveformTable::~LFOWaveformTable()
{
}

void LFOWaveformTable::prepareToPlay()
{
    // set frequency and wave form
    frequency = 5.0;
    waveForm = sineWave;
}

void LFOWaveformTable::setSampleRate(double sampleRate)
{
    currentSampleRate = sampleRate;
}

// Initialize our table buffer
void LFOWaveformTable::fillLFOTable(float *table)
{
    // Initialize sine waveform and put into tableBuf
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        tableBuf[i] = table[i];
    }
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
        _nextOutput = tableBuf[_outPoint] * _omAlpha;
        // second part of interpolation (fractional)
        if (_outPoint + 1 < _maxDelay) {
            _nextOutput += tableBuf[_outPoint+1] * _alpha;
        }
        else {
            _nextOutput += _sampBuffer[0] * _alpha;
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
    
    // read follows write
    double outPointer = _inPoint - increment;
    _delay = increment;
    
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


float LFOWaveformTable::generateWaveSample()
{
    float waveSample;
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
    else {
        waveSample = generateLFOTable(frequency); // implement wavetable later
    }
    
    return waveSample;
}

// Stock Waveform Generation
float LFOWaveformTable::generateSine(float freq)
{
    float phase, sinSample = 0;
    static float prev_phase;
    
    phase = 2 * M_PI * freq / currentSampleRate + prev_phase;
    sinSample = sin(phase);
    
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
    static float sawSample;
    
    float T = currentSampleRate / freq;
    
    sawSample += 2./T;
    triSample = fabsf(sawSample) * 2 - 1;
    
    if (sawSample >= 1)
    {
        sawSample -= 2;
    }
    
    return triSample;
}

float LFOWaveformTable::generateSawtooth(float freq)
{
    static float sawSample;
    float T = currentSampleRate / freq;

    sawSample += 2./T;
    
    if (sawSample >= 1)
    {
        sawSample -= 2;
    }
    
    return sawSample;
    
}

float LFOWaveformTable::generateSquare(float freq)
{
    float phase, sawSample = 0;
    static float prev_phase;
    
    phase = 2 * M_PI * freq / currentSampleRate + prev_phase;
    sawSample = sin(phase);
    if(sawSample >= 0.2){
        sawSample = 0.2;
    }
    else if (sawSample <= -0.2){
        sawSample = -0.2;
    }
    
    sawSample *= 5;
    
    if (phase > (2 * M_PI))
    {
        phase -= (2 * M_PI);
    }
    
    prev_phase = phase;
    
    return sawSample;
}

float LFOWaveformTable::generateLFOTable(float freq)
{
    float tableSample;
    // Number of samples per period
    float T = currentSampleRate / freq;
    static float tableIncr = 0.0;
    
    // Fractionally increment the table value
    tableIncr += (double)(_maxDelay / T);
    
    // wrap the table increment
    if (tableIncr >= 1024.0) {
        tableIncr = 0.0;
    }
    setIncrement(tableIncr);
    
    tableSample = tableLookup();
    
    return tableSample;
}






