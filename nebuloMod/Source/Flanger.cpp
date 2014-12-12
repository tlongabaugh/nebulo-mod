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
    
    min_dly = 0;
    max_dly = 7;
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

/*
 * Flanger Equation: y[n] = x[n] + wetness*x[n-delay] + feedback*y[n-1]
 * delay = 0.5*sample_rate / ( center_frequency + mod_depth* LFO[phasor] )
 */

// MONO AUDIO PROCESSING
void Flanger::processMonoSamples(float* const samples, const int numSamples)
{
    // Make sure left channel is not NULL
    jassert (samples != nullptr);
    
    static float phase = 0;
    static int delay_buf_len = 0;
    static int dly_wr_ptr = 0;
    float *delay_buffer;

    for (int i = 0; i < numSamples; ++i)
    {
        const float input = samples[i];
        float delta = 0.0f;
        
        float current_delay = (0.5f + 0.5f * sinf(2.0 * M_PI * phase));
        
        float dly_rd_ptr = fmodf((float)dly_wr_ptr - (float)(current_delay * 44100) + (float)(delay_buf_len - 3.0), (float)delay_buf_len);
        
        // Interpolation!!!
        float fraction = dly_rd_ptr = floorf(dly_rd_ptr);
        int prev_sample = (int)floorf(dly_rd_ptr);
        int next_sample = (prev_sample + 1) % delay_buf_len;
        delta = fraction * delay_buffer[next_sample] + (1.0f - fraction) * delay_buffer[prev_sample];
        
        // Store current information in delay buffer
        delay_buffer[dly_wr_ptr] = input + (delta * params.feedback);
        
        if (++dly_wr_ptr >= delay_buf_len)
            dly_wr_ptr = 0;
        
        // Output is this:
        samples[i] = input + params.depth * delta;
        
        // Wrap Phase
        phase += params.rate * 1/44100;
        if (phase >= 1.0)
            phase -= 1.0;
    }
}

// STEREO AUDIO PROCESSING
void Flanger::processStereoSamples(float* const left, float* const right, const int numSamples)
{
    // Make sure left and right channels are not NULL
    jassert (left != nullptr && right != nullptr);
    
    static float phase = 0;
    static int delay_buf_len = 0;
    static int dly_wr_ptr = 0;
    float *delay_buffer;
    
    // Process stereo samples
    for (int i = 0; i < numSamples; ++i)
    {
        const float input = left[i] + right[i];
        float delta = 0.0f;
        
        float current_delay = (0.5f + 0.5f * sinf(2.0 * M_PI * phase));
        
        float dly_rd_ptr = fmodf((float)dly_wr_ptr - (float)(current_delay * 44100) + (float)(delay_buf_len - 3.0), (float)delay_buf_len);
        
        // Interpolation!!!
        float fraction = dly_rd_ptr = floorf(dly_rd_ptr);
        int prev_sample = (int)floorf(dly_rd_ptr);
        int next_sample = (prev_sample + 1) % delay_buf_len;
        delta = fraction * delay_buffer[next_sample] + (1.0f - fraction) * delay_buffer[prev_sample];
        
        // Store current information in delay buffer
        delay_buffer[dly_wr_ptr] = input + (delta * params.feedback);
        
        if (++dly_wr_ptr >= delay_buf_len)
            dly_wr_ptr = 0;
        
        // Output is this:
        left[i] = input + params.depth * delta;
        right[i] = input + params.depth * delta;
        
        // Wrap Phase
        phase += params.rate * 1/44100;
        if (phase >= 1.0)
            phase -= 1.0;
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