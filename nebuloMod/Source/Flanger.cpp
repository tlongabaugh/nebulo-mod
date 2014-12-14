//
//  Flanger.cpp
//  NebuloMod
//
//  Created by Ryan Foo on 11/23/14.
//
//

#include "Flanger.h"

// Constructor
Flanger::Flanger(void) : currentSampleRate(SAMPLE_RATE)
{
    // Set Paramaters
    setParameters(Parameters());
    // Set Sample Rate (44100 default)
    setSampleRate(SAMPLE_RATE);
    
    //min_dly = 0;
    //max_dly = 7;
}

// Deconstructor
Flanger::~Flanger(void)
{
    delete [] delayline;
  /*  if (input_buffer)
    {
        delete [] input_buffer;
    }
    if (output_buffer)
    {
        delete [] output_buffer;
    }*/
}

/*
 * Flanger Equation: y[n] = x[n] + wetness*x[n-delay] + feedback*y[n-1]
 * delay = 0.5*sample_rate / ( center_frequency + mod_depth* LFO[phasor] )
 */

// MONO AUDIO PROCESSING
void Flanger::processMono(float* const samples, const int numSamples)
{
    // Make sure left channel is not NULL
    jassert (samples != nullptr);
    
    /*
    float lfoSample;
    for(int i = 0; i < numSamples; i++) {
        // get lfo sample, process mono channel
        lfoSample = LFO.generateWaveSample();
        samples[i] = processSample(samples[i], lfoSample);
    }*/
    
    float val;
    float delayed;
    fwdhop = ((delaysize*rate*2)/currentSampleRate) + 1.0f;
    //fwdhop = delaysize*lfoSample + 1.0f;
    
    for(int i=0; i<numSamples; i++) {
        // write to the delay line
        val = samples[i];
        delayline[writepos++] = val;
        if (writepos == delaysize) {
            writepos = 0;
        }
        
        // read from the delay line
        delayed =  delayline[(int)readpos];
        readpos += fwdhop;
        
        // update pos, could be going forward or backward
        while ((int)readpos >= delaysize) {
            readpos -= delaysize;
        }
        while ((int)readpos < 0) {
            readpos += delaysize;
        }
        
        // mix
        samples[i] = (val + (delayed * depth)) * gain;

    }

    /*
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
    }*/
}

// STEREO AUDIO PROCESSING
void Flanger::processStereo(float* const left, float* const right, const int numSamples)
{
    // Make sure left and right channels are not NULL
    jassert (left != nullptr && right != nullptr);
    
    float val;
    float delayed;
    fwdhop = ((delaysize*rate*2)/currentSampleRate) + 1.0f;
    //fwdhop = delaysize*lfoSample + 1.0f;
    
    for(int i=0; i<numSamples; i++) {
        // write to the delay line
        val = (left[i] + right[i]) / 2.0;
        delayline[writepos++] = val;
        if (writepos == delaysize) {
            writepos = 0;
        }
        
        // read from the delay line
        delayed =  delayline[(int)readpos];
        readpos += fwdhop;
        
        // update pos, could be going forward or backward
        while ((int)readpos >= delaysize) {
            readpos -= delaysize;
        }
        while ((int)readpos < 0) {
            readpos += delaysize;
        }
        
        // mix
        left[i] = (val + (delayed * depth)) * gain;
        right[i] = (val + (delayed * depth)) * gain;
        
    }
   /* float lfoSample;
    for(int i = 0; i < numSamples; i++) {
        // Get lfo sample
        lfoSample = LFO.generateWaveSample();
        
        // process left and right channels
        left[i] = processSample(left[i], lfoSample);
        right[i] = processSample(right[i], lfoSample);
    }*/
   /*
    
    
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
    }*/
}
/*
void Flanger::processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames)
{
    
    float val;
    float delayed;
    
    for(int n=0; n<NUMCHANS; n++)
    { fwdhop[n] = ((delaysize[n]*rate[n]*2)/sampleRate) + 1.0f; }
    
    for(int i=0; i<sampleFrames; i++) {
        for(int n=0; n<NUMCHANS; n++) {
            val[n] = inputs[n][i];
            
            // write to delay ine
            delayline[n][writepos[n]++] = val[n];
            if(writepos[n]==delaysize[n]) { writepos[n] = 0; }
            
            // read from delay ine
            delayed[n] = delayline[n][(int)readpos[n]];
            readpos[n] += fwdhop[n];
            
            // update pos, could be going forward or backward
            while((int)readpos[n] >= delaysize[n]) { readpos[n] -= delaysize[n]; }
            while((int)readpos[n] < 0) { readpos[n] += delaysize[n]; }
            
            // mix
            outputs[n][i] = (val[n] + (delayed[n] * depth[n])) * gain[n];
        }
    }
}*/
/*
float Flanger::processSample(float input, float lfoSample)
{
    float delay = 800;
    //todo: needs fixing
    float output;
    output = dl.dl(input, delay + (lfoSample * parameters.depth * delay) + 1, parameters.feedback) ;
    float normalise = (1 - fabs(output));
    output *= normalise;
    return (output + input) / 2.0;
}*/




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
    
    depth = .75f;
    rate = 10.f;
    gain = 1.f;
    delaysize = currentSampleRate * 0.02f;
    delta = (delaysize * rate) / currentSampleRate;
    fwdhop = delta + 1.0f;
    writepos = 0;
    readpos = 0;
    delayline = new float[(int)delaysize];
    memset(delayline, 0, delaysize*sizeof(float));

}

// flush the buffers and parameters once we exit DAW
void Flanger::flush(void)
{/*
    input_buffer = NULL;
    output_buffer = NULL;*/
}