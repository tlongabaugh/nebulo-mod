//
//  Phaser.cpp
//  NebuloMod
//
//  Created by Ryan Foo on 11/23/14.
//
//

#include "Phaser.h"

// Constructor
Phaser::Phaser() : currentSampleRate(SAMPLE_RATE),
lfoPhase(0.f),
depth(.1f),
zm1(.1f)
{
    setParameters(Parameters());
    setSampleRate(SAMPLE_RATE);
    // default Program Values
    
    setRange(1500.f, 1600.f);
    setRate(0.5f);
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

void Phaser::setRange(float freqMin, float freqMax)
{ // Hz
    depthMin = freqMin / (SAMPLE_RATE/2.f);
    depthMax = freqMax / (SAMPLE_RATE/2.f);
}

void Phaser::setRate(float rate)
{ // cps
    lfoInc = 2.f * M_PI * (rate / SAMPLE_RATE);
}

void Phaser::setDepth(float newDepth)
{
    depth = newDepth;
}

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
    /*
    //calculate and update phaser sweep lfo...
    float delay  = depthMin + (depthMax - depthMin) * ((sin(lfoPhase) + 1.f)/2.f);
    lfoPhase += lfoInc;
    if(lfoPhase >= M_PI * 2.f )
        lfoPhase -= M_PI * 2.f;
    
    //update filter coeffs
    for(int i = 0; i < 6; i++) {
        allPass[i].delayAmt(delay);
    }
    
    //calculate output
    float y = allPass[0].update(allPass[1].update(allPass[2].update(allPass[3].update(allPass[4].update(allPass[5].update(newSample + zm1))))));
    
    zm1 = y;
    
    return newSample + y * depth;*/
    
    return newSample;
}
