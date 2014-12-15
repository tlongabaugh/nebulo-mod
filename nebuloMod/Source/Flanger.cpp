//
//  Flanger.cpp
//  NebuloMod
//
//  Created by Ryan Foo on 11/23/14.
//
//

#include "Flanger.h"

// Constructor
Flanger::Flanger(void) : currentSampleRate(INIT_SAMPLE_RATE)
{
    // Set Paramaters
    setParameters(Parameters());
    // Set Sample Rate (44100 default, updated right before playback)
    setSampleRate(INIT_SAMPLE_RATE);
    
    delayLine = new DelayLine(2047, 4095);
    
}

// Deconstructor
Flanger::~Flanger(void)
{

}

// MONO AUDIO PROCESSING
void Flanger::processMono(float* const samples, const int numSamples)
{
    // Make sure left channel is not NULL
    jassert (samples != nullptr);
    for (int i = 0; i < numSamples; i++) {
        samples[i] = (samples[i] + delayLine->processSample(samples[i]))/2.0;
    }
    

}

// STEREO AUDIO PROCESSING
void Flanger::processStereo(float* const left, float* const right, const int numSamples)
{
    // Make sure left and right channels are not NULL
    jassert (left != nullptr && right != nullptr);
    for (int i = 0; i < numSamples; i++) {
        left[i] = (left[i] + delayLine->processSample(left[i]))/2.0;
        right[i] = (right[i] + delayLine->processSample(right[i]))/2.0;
    }
    
}




void Flanger::prepareToPlay()
{
    // Init LFO
    LFO.setSampleRate(currentSampleRate);
    LFO.prepareToPlay();
    
}

// Get the parameters of the flanger
Flanger::Parameters& Flanger::getParameters(void)
{
    return parameters;
}

// Set the parameters of the flanger
void Flanger::setParameters(const Parameters& newParam)
{
    parameters = newParam;
}

// Set the sample rate of the flanger
void Flanger::setSampleRate (const double sampleRate)
{
    // Make sure sample rate is above 0 (NO NEGATIVE VALUES ACCEPTED)
    jassert (sampleRate > 0);
    currentSampleRate = sampleRate;

}

// flush the buffers and parameters once we exit DAW
void Flanger::flush(void)
{/*
    input_buffer = NULL;
    output_buffer = NULL;*/
}