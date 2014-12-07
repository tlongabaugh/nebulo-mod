//
//  Flanger.cpp
//  NebuloMod
//
//  Created by Ryan Foo on 11/23/14.
//
//

#include "Flanger.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>

// Constructor
Flanger::Flanger(void)
{
    // Setting Parameter Knobs/Sliders
    paramDepth = 0.5f;
    paramRate = 0.5f;
    paramLFO = 0.5f;
    paramLFOWaveform = 0;
    paramResonance = 0.5f;
    paramManCtrl = 0.5f;
    paramMix = 0.5f;
    // paramMixMode = 0.5f;
    
    // Actual Values (need to change numbers
    depth = 0.5;
    rate = 0.5;
    lfo = 0.5;
    lfoWaveform = 0;
    resonance = 0.5;
    manCtrl = 0.5;
    mix = 0.5;
    mixMode = 0;
    
    // Create Buffers
    input_buffer = new double[BSZ];
    output_buffer = new double[BSZ];
    for (int i = 0; i < BSZ; i++)
    {
        input_buffer[i] = output_buffer[i] = 0.0;
    }
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

// Set Depth
void Flanger::setDepth(float depth)
{
    
}

// Set Rate
void Flanger::setRate(float rate)
{
    
}

// Set LFO
void Flanger::setLFO(float lfo, float waveform)
{
    
}

// Set Resonance
void Flanger::setResonance(float resonance)
{
    
}

// Set Manual Control
void Flanger::setManualControl(float manCtrl)
{
    
}

// Set Mix
void Flanger::setMix(float mix)
{
    
}

// Set Mix Mode
void Flanger::setMixMode(float mode)
{
    
}