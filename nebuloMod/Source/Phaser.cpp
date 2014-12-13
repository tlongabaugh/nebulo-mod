//
//  Phaser.cpp
//  NebuloMod
//
//  Created by Ryan Foo on 11/23/14.
//
//  Written by Tom Longabaugh, with help from Will Pirkle's "Designing Audio Effects in C++" book

#include "Phaser.h"

// Constructor
Phaser::Phaser() : currentSampleRate(SAMPLE_RATE)
{
    setParameters(Parameters());
    setSampleRate(SAMPLE_RATE);
    
    minFreqAPF_1 = 16.0;
    maxFreqAPF_1 = 1600.0;
    
    minFreqAPF_2 = 33.0;
    maxFreqAPF_2 = 3300.0;
    
    minFreqAPF_3 = 48.0;
    maxFreqAPF_3 = 48.0;
}

Phaser::~Phaser(void)
{
}

void Phaser::setParameters (const Parameters& newParams)
{
    parameters = newParams;
}


void Phaser::setSampleRate (const double sampleRate)
{
    // Make sure sample rate is more than 0
    jassert(sampleRate > 0);
    currentSampleRate = sampleRate;
}

float Phaser::calculateAPFCutoffFreq(float LFOsample, float minFreq, float maxFreq)
{
    return LFOsample + (maxFreq - minFreq) + minFreq;
}

void Phaser::calculateFirstOrderAPFCoeffs(float cutoffFreq, BiQuad* BiQuadFilter)
{
    //coefficient calculation
    float alphaNum = tan(M_PI * cutoffFreq/(float)SAMPLE_RATE) - 1.0;
    float alphaDen = tan(M_PI * cutoffFreq/(float)SAMPLE_RATE) + 1.0;
    float alpha = alphaNum/alphaDen;
    
    //Set the coefficients for this filter
    BiQuadFilter->a0 = alpha;
    BiQuadFilter->a1 = 1.0;
    BiQuadFilter->a1 = 0.0;
    BiQuadFilter->b1 = alpha;
    BiQuadFilter->b2 = alpha;
    
}

void Phaser::calculateFirstOrderAPFCoeffsLeft(float LFOsample)
{
    // All pass 1
    float cutoffFreq = calculateAPFCutoffFreq(LFOsample, minFreqAPF_1, maxFreqAPF_1);
    calculateFirstOrderAPFCoeffs(cutoffFreq, &leftAPF_1);
    
    // All pass 2
    cutoffFreq = calculateAPFCutoffFreq(LFOsample, minFreqAPF_2, maxFreqAPF_2);
    calculateFirstOrderAPFCoeffs(cutoffFreq, &leftAPF_2);
    
    // All pass 3
    cutoffFreq = calculateAPFCutoffFreq(LFOsample, minFreqAPF_3, maxFreqAPF_3);
    calculateFirstOrderAPFCoeffs(cutoffFreq, &leftAPF_3);
}

void Phaser::calculateFirstOrderAPFCoeffsRight(float LFOsample)
{
    // All pass 1
    float cutoffFreq = calculateAPFCutoffFreq(LFOsample, minFreqAPF_1, maxFreqAPF_1);
    calculateFirstOrderAPFCoeffs(cutoffFreq, &rightAPF_1);
    
    // All pass 2
    cutoffFreq = calculateAPFCutoffFreq(LFOsample, minFreqAPF_2, maxFreqAPF_2);
    calculateFirstOrderAPFCoeffs(cutoffFreq, &rightAPF_2);
    
    // All pass 3
    cutoffFreq = calculateAPFCutoffFreq(LFOsample, minFreqAPF_3, maxFreqAPF_3);
    calculateFirstOrderAPFCoeffs(cutoffFreq, &rightAPF_3);
}

void Phaser::prepareToPlay()
{
    leftAPF_1.flushDelays();
    rightAPF_1.flushDelays();
    leftAPF_2.flushDelays();
    rightAPF_2.flushDelays();
    leftAPF_3.flushDelays();
    rightAPF_3.flushDelays();
    
    LFO.m_fFrequency_Hz = 440;
    LFO.m_uPolarity = 1;
    LFO.m_uTableMode = 0;
    LFO.m_uOscType = 0;
    LFO.setSampleRate(SAMPLE_RATE);
    LFO.prepareForPlay();
    
}


//TODO: initialize LFO, set it's values
//TODO: hook up UI
//todo: finish processaudio function and get that working

//================================================================================================================

void Phaser::processStereo(float* const left, float* const right, const int numSamples) noexcept
{
    float yn = 0; // to hold lfo output
    float yqn = 0; //quad output (not used)
    float what;
    for(int i = 0; i < numSamples; i++) {
        // get lfo sample
        LFO.doOscillate(&yn, &yqn);
        //left[i] = yn;
        //right[i] = yn;
        //what = processAudioFrame(left[i], yn, 1);
        left[i] = processAudioFrame(left[i], yn, 1);
        right[i] = processAudioFrame(right[i], yn, 1);

    }
}

void Phaser::processMono(float* const samples, const int numSamples) noexcept
{
    float yn = 0; // to hold lfo output
    float yqn = 0; //quad output (not used)
    
    for(int i = 0; i < numSamples; i++) {
        // get lfo sample
        LFO.doOscillate(&yn, &yqn);
        //samples[i] = yn;
        samples[i] = processAudioFrame(samples[i], yn, 1);
    }
}

float Phaser::processAudioFrame(float inputSample, float lfoSample, int numChannels)
{
    // APFs
    float APF1, APF2, APF3;
    float outputSample;

    // calculate depth (100% when control is 50%)
    float depth = 50.0/200.0;

    calculateFirstOrderAPFCoeffsLeft(lfoSample);
    APF1 = leftAPF_1.doBiQuad(inputSample);
    APF2 = leftAPF_2.doBiQuad(APF1);
    APF3 = leftAPF_3.doBiQuad(APF2);
    
    outputSample = depth * APF3 + (1.0 - depth)*inputSample;
    
    // BUSTED RIGHT NOW
    //return outputSample;
    
    return inputSample;
    
    //calculateFirstOrderAPFCoeffsRight(lfoSample);

    
}
