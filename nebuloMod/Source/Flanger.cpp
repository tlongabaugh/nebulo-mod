//
//  Flanger.cpp
//  NebuloMod
//
//  Written by Tom Longabaugh
//
//

#include "Flanger.h"

Flanger::Flanger(void) : currentSampleRate(INIT_SAMPLE_RATE), avgDelay(100.0)
{
    // Set Paramaters
    setParameters(Parameters());
    // Set Sample Rate (44100 default, updated right before playback)
    setSampleRate(INIT_SAMPLE_RATE);
    
    //delayLineL = new DelayLine(100, 4095);
    //delayLineR = new DelayLine(100, 4095);
    
    delayLineL.setDelay(4090);
    delayLineR.setDelay(4090);
    
}

Flanger::~Flanger(void)
{
    ///delete [] delayLineL;
    //delete [] delayLineR;
}

void Flanger::setParameters(const Parameters& newParam)
{
    parameters = newParam;
    
    // change LFO frequency if it's between our bounds (we should be checking/scaling for this anyways in GUI)
    if (parameters.rate >= 1.0f && parameters.rate <= 15.0f) {
        LFO.frequency = parameters.rate;
    }
    // change lfo waveform type if it's a valid type
    if (parameters.lfoWaveform >= 0 && parameters.lfoWaveform <= 4) {
        LFO.waveForm = parameters.lfoWaveform;
    }
}

void Flanger::setSampleRate (const double sampleRate)
{
    // Make sure sample rate is valid
    jassert (sampleRate > 0);
    currentSampleRate = sampleRate;
    _maxFlanging = 0.02 * currentSampleRate;
}

void Flanger::prepareToPlay()
{
    // Init LFO
    LFO.setSampleRate(currentSampleRate);
    LFO.prepareToPlay();
    
    // reset delay line
    delayLineL.clear();
    delayLineR.clear();
}


//AUDIO PROCESSING (stereo, mono, which both call processSamples())
//================================================================================================================

void Flanger::processMono(float* const samples, const int numSamples)
{
    // Make sure left channel is not NULL
    jassert (samples != nullptr);
    
    float lfoSample;
    
    for (int i = 0; i < numSamples; i++) {
        // Get LFO sample
        LFO.waveForm = parameters.lfoWaveform;
        lfoSample = LFO.generateWaveSample() + 1.0; // get into correct non-negative range
        
        // process samples
        delayLineL.setDelay(avgDelay*lfoSample*10);
        //samples[i] = samples[i]*(1.0-parameters.mix/2) + parameters.depth*(delayLineL->processSample(samples[i]))*parameters.mix/2;
        
    }
}

void Flanger::processStereo(float* const left, float* const right, const int numSamples)
{
    // Make sure left and right channels are not NULL
    jassert (left != nullptr && right != nullptr);
    float phase;
    static float prev_phase;
    static float triSample;
    static float counter = 1;
    
    float lfoSample;
    
    for (int i = 0; i < numSamples; i++) {
        /*phase = 2 * M_PI * (parameters.rate*100) / currentSampleRate + prev_phase;
        lfoSample = (sin(phase) + 1.0) / 2.0;
        if (phase > (2 * M_PI))
        {
            phase -= (2 * M_PI);
        }
        
        prev_phase = phase;*/
        
        // Get LFO sample
        LFO.waveForm = parameters.lfoWaveform;
        //LFO.frequency = 40.0;
        lfoSample = (LFO.generateWaveSample() + 1.0)/2.0; // get into 0-1 range
        
        // process samples
        //double delay = i/2000.0 * _maxFlanging;
        
        double delay = (lfoSample * _maxFlanging);//   avgDelay*lfoSample*10;
        delayLineL.setDelay(delay);
        delayLineR.setDelay(delay);
        
        
        right[i] = parameters.depth*delayLineR.processSample(right[i]);
        left[i] = left[i];
        //left[i] = (left[i] + parameters.depth * delayLineL.processSample(left[i]))/2.0; // make this mix better
        //right[i] = (right[i] + parameters.depth * delayLineR.processSample(right[i]))/2.0; // make this mix better
        
        
        //left[i] = left[i]*(1.0-parameters.mix/2) + parameters.depth*(delayLineL->processSample(left[i]))*parameters.mix/2;
        //right[i] = right[i]*(1.0-parameters.mix/2) + parameters.depth*(delayLineR->processSample(right[i]))*parameters.mix/2;
        
    }
}







