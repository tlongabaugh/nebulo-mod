//
//  Flanger.cpp
//  NebuloMod
//
//  Created by Ryan Foo on 11/23/14.
//
//

#include "Flanger.h"

Flanger::Flanger(void) : currentSampleRate(INIT_SAMPLE_RATE)
{
    // Set Paramaters
    setParameters(Parameters());
    // Set Sample Rate (44100 default, updated right before playback)
    setSampleRate(INIT_SAMPLE_RATE);
    
    delayLine = new DelayLine(2047, 4095);
    
}

Flanger::~Flanger(void)
{
    delete [] delayLine;
}

void Flanger::setParameters(const Parameters& newParam)
{
    parameters = newParam;
}

void Flanger::setSampleRate (const double sampleRate)
{
    // Make sure sample rate is valid
    jassert (sampleRate > 0);
    currentSampleRate = sampleRate;
}

void Flanger::prepareToPlay()
{
    // Init LFO
    LFO.setSampleRate(currentSampleRate);
    LFO.prepareToPlay();
    
    // reset delay line
    delayLine->clear();
    
}



//AUDIO PROCESSING (stereo, mono, which both call processSamples())
//================================================================================================================

void Flanger::processMono(float* const samples, const int numSamples)
{
    // Make sure left channel is not NULL
    jassert (samples != nullptr);
    for (int i = 0; i < numSamples; i++) {
        samples[i] = (samples[i] + delayLine->processSample(samples[i]))/2.0;
    }
    

}

void Flanger::processStereo(float* const left, float* const right, const int numSamples)
{
    // Make sure left and right channels are not NULL
    jassert (left != nullptr && right != nullptr);
    for (int i = 0; i < numSamples; i++) {
        left[i] = (left[i] + delayLine->processSample(left[i]))/2.0;
        right[i] = (right[i] + delayLine->processSample(right[i]))/2.0;
    }
    
}


