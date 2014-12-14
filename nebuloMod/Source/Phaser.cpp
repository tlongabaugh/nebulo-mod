//
//  Phaser.cpp
//  NebuloMod
//
//  Created by Ryan Foo on 11/23/14.
//
//  Written by Tom Longabaugh, with help from Will Pirkle's "Designing Audio Effects in C++" book

#include "Phaser.h"

// Constructor
Phaser::Phaser() : currentSampleRate(SAMPLE_RATE), _depth(1.0), _zm1( 0.f )
{
    setParameters(Parameters());
    setSampleRate(SAMPLE_RATE);

    Range( 440.f, 1600.f );
}

Phaser::~Phaser()
{
}

void Phaser::Range(float fMin, float fMax){ // Hz
    _dmin = fMin / (currentSampleRate/2.f);
    _dmax = fMax / (currentSampleRate/2.f);
}

void Phaser::Feedback(float fb){ // 0 -> <1.
    _fb = fb;
}

void Phaser::Depth(float depth){  // 0 -> 1.
    _depth = depth;
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

void Phaser::prepareToPlay()
{
    // Init LFO
    LFO.setSampleRate(currentSampleRate);
    LFO.prepareToPlay();
    
}

//================================================================================================================

void Phaser::processStereo(float* const left, float* const right, const int numSamples) noexcept
{
    float lfoSample;
    for(int i = 0; i < numSamples; i++) {
        lfoSample = LFO.generateWaveSample();
        left[i] = processSample(left[i], lfoSample);
        right[i] = processSample(right[i], lfoSample);
    }
}

void Phaser::processMono(float* const samples, const int numSamples) noexcept
{
    jassert (samples != nullptr);
    
    float lfoSample;
    for(int i = 0; i < numSamples; i++) {
        lfoSample = LFO.generateWaveSample();
        samples[i] = processSample(samples[i], lfoSample);
    }
}

float Phaser::processSample(float inSamp, float lfoSample)
{
    //calculate and update phaser sweep lfo...
    float d  = _dmin + (_dmax-_dmin) * (lfoSample + 1.f)/2.f;
    
    //update filter coeffs
    for( int i=0; i<6; i++ )
        allPass[i].delay( d );
    
    //calculate output
    float y = allPass[0].update(allPass[1].update(allPass[2].update(allPass[3].update(allPass[4].update(allPass[5].update(inSamp + _zm1 * _fb ))))));
    _zm1 = y;
    
    return inSamp + y * _depth;
}

