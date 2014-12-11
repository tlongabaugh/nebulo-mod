//
//  Flanger.cpp
//  NebuloMod
//
//  Created by Ryan Foo on 11/23/14.
//
//

#include "Flanger.h"

// Constructor
Flanger::Flanger(void)
{
    // Set Paramaters
    setParameters(Parameters());
    
    // Set Sample Rate (44100 default)
    setSampleRate(SAMPLE_RATE);
}

// Deconstructor
Flanger::~Flanger(void)
{
    if (input_buffer)
    {
        delete [] input_buffer;
    }
    if (output_buffer)
    {
        delete [] output_buffer;
    }
}

// MONO AUDIO PROCESSING
void Flanger::processMonoSamples(float* const samples, const int numSamples)
{
    // Make sure left channel is not NULL
    jassert (samples != nullptr);
    
    for (int i = 0; i < numSamples; ++i)
    {
        const float input = samples[i] * gain;
        float output = 0;
        
        samples[i] = output * 1 * wet + samples[i] * dry;
    }
}

// STEREO AUDIO PROCESSING
void Flanger::processStereoSamples(float* const left, float* const right, const int numSamples)
{
    // Make sure left and right channels are not NULL
    jassert (left != nullptr && right != nullptr);
    
    // Process stereo samples
    for (int i = 0; i < numSamples; ++i)
    {
        const float input = (left[i] + right[i]) * gain;
        float outL = 0, outR = 0;
        
        left[i] = outL * wet + left[i] * dry;
        left[i] = outR * wet + right[i] * dry;
    }
}

// Get the parameters of the flanger
Flanger::Parameters& Flanger::getParameters(void)
{
    return params;
}

// Set the parameters of the flanger
void Flanger::setParameters(const Parameters& newParam)
{
    params = newParam;
}

// Set the sample rate of the flanger
void Flanger::setSampleRate (const double sampleRate)
{
    // Make sure sample rate is above 0 (NO NEGATIVE VALUES ACCEPTED)
    jassert (sampleRate > 0);
}

// flush the buffers and parameters once we exit DAW
void Flanger::flush(void)
{
    input_buffer = NULL;
    output_buffer = NULL;
}