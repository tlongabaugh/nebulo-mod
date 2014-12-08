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
    setParameters(params);
    
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
    // Buffer size is 2048
    for(int i = 0; i < 2048; i++)
    {
        
    }
}

// STEREO AUDIO PROCESSING
void Flanger::processStereoSamples(float* const left, float* const right, const int numSamples)
{
    // Buffer size is 2048
    for (int i = 0; i < 2048; i++)
    {
        
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
    
}

// flush the buffers and parameters once we exit DAW
void Flanger::flush(void)
{
    input_buffer = NULL;
    output_buffer = NULL;
}

// Set Depth
void Flanger::setDepth(float depth)
{
    params.depth = depth;
}

// Set Rate
void Flanger::setRate(float rate)
{
    params.rate = rate;
}

// Set LFO
void Flanger::setLFOWaveform(int waveform)
{
    params.lfoWaveform = waveform;
}

// Set Resonance
void Flanger::setResonance(float resonance)
{
    params.resonance = resonance;
}

// Set Mix
void Flanger::setMix(float mix)
{
    params.mix = mix;
}