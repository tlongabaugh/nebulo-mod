//
//  Flanger.cpp
//  NebuloMod
//
//  Written by Tom Longabaugh
//
//

#include "Flanger.h"

Flanger::Flanger(void) : currentSampleRate(INIT_SAMPLE_RATE)
{
    // Set Paramaters and sample rate (44100 default, updated right before playback)
    setParameters(Parameters());
    setSampleRate(INIT_SAMPLE_RATE);

}

Flanger::~Flanger()
{
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
    static float dOut;
    
    for (int i = 0; i < numSamples; i++) {
        // Get LFO sample
        LFO.waveForm = parameters.lfoWaveform;
        lfoSample = (LFO.generateWaveSample() + 1.0)/2.0; // get into 0-1 range
        
        // change delay time
        double delay = (lfoSample * _maxFlanging);
        delayLineL.setDelay(delay);
        
        // calculate delayed sample while also putting feedback into the beginning of delay line
        dOut = delayLineL.processSample((samples[i] + dOut*parameters.feedback));
        
        // build output mix
        samples[i] = samples[i]*(1.0-parameters.mix) + parameters.depth*dOut*parameters.mix;
        
    }
}

void Flanger::processStereo(float* const left, float* const right, const int numSamples)
{
    // Make sure left and right channels are not NULL
    jassert (left != nullptr && right != nullptr);
    
    float lfoSample;
    static float dOutL = 0;
    static float dOutR = 0;
    
    for (int i = 0; i < numSamples; i++) {
        // Get LFO sample
        LFO.waveForm = parameters.lfoWaveform;
        lfoSample = (LFO.generateWaveSample() + 1.0)/2.0; // get into 0-1 range

        // change delay time
        double delay = (lfoSample * _maxFlanging);
        delayLineL.setDelay(delay);
        delayLineR.setDelay(delay);

        // calculate delayed sample while also putting feedback into the beginning of delay line
        dOutL = delayLineL.processSample((left[i] + dOutL*parameters.feedback));
        dOutR = delayLineR.processSample((right[i] + dOutR*parameters.feedback));
        
        // build output mix
        left[i] = left[i]*(1.0-parameters.mix) + parameters.depth*dOutL*parameters.mix;
        right[i] = right[i]*(1.0-parameters.mix) + parameters.depth*dOutR*parameters.mix;
    }
}







