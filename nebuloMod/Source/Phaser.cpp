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

float calculateAPFCutoffFreq(float LFOsample, float minFreq, float maxFreq)
{
    return LFOsample + (maxFreq - minFreq) + minFreq;
}

void calculateFirstOrderAPFCoeffs(float cutoffFreq, BiQuad* BiQuadFilter)
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


void Phaser::reset()
{

}


//================================================================================================================

void Phaser::processStereo(float* const left, float* const right, const int numSamples) noexcept
{
    for(int i = 0; i < numSamples; i++) {
       left[i] = processSingleSample(left[i]);
       right[i] = processSingleSample(right[i]);
    }
    
}

void Phaser::processMono(float* const samples, const int numSamples) noexcept
{
    for(int i = 0; i < numSamples; i++) {
        samples[i] = processSingleSample(samples[i]);
    }
}

float Phaser::processSingleSample (float newSample) noexcept
{
    //calculate and update phaser sweep lfo...
    float delay  = depthMin + (depthMax - depthMin) * ((sin(lfoPhase) + 1.f)/2.f);
    lfoPhase += lfoInc;
    if(lfoPhase >= M_PI * 2.f )
        lfoPhase -= M_PI * 2.f;
    
    //update filter coeffs
    for(int i = 0; i < 4; i++) {
        allPass[i].delayAmt(delay);
    }
    
    //calculate output
    float y = allPass[0].update(allPass[1].update(allPass[2].update(allPass[3].update(newSample + zm1))));
    
    zm1 = y;
    
    return newSample + y * depth;
    

}
