//
//  Phaser.cpp
//  NebuloMod
//
//  Written by Tom Longabaugh

#include "Phaser.h"

// Constructor
Phaser::Phaser() : currentSampleRate(INIT_SAMPLE_RATE), zm1( 0.f )
{
    setParameters(Parameters());
    setSampleRate(INIT_SAMPLE_RATE);

    range(440.f, 1600.f);
}

Phaser::~Phaser()
{
}

void Phaser::range(float fMin, float fMax){ // Hz
    depthMin = fMin / (currentSampleRate/2.f);
    depthMax = fMax / (currentSampleRate/2.f);
}

void Phaser::setParameters (const Parameters& newParams)
{
    parameters = newParams;
    
    // change LFO frequency if it's between our bounds (we should be checking/scaling for this anyways in GUI)
    if (parameters.rate >= 1.0f && parameters.rate <= 15.0f) {
        LFO.frequency = parameters.rate;
    }
    // change lfo waveform type if it's a valid type
    if (parameters.lfoWaveform >= 0 && parameters.lfoWaveform <= 4) {
        LFO.waveForm = parameters.lfoWaveform;
    }
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

//AUDIO PROCESSING (stereo, mono, which both call processSample())
//================================================================================================================

void Phaser::processMono(float* const samples, const int numSamples) noexcept
{
    // Make sure samples channel is not NULL
    jassert (samples != nullptr);
    
    float lfoSample;
    
    for(int i = 0; i < numSamples; i++) {
        // get lfo sample, process mono channel
        LFO.waveForm = parameters.lfoWaveform;
        lfoSample = LFO.generateWaveSample();
        
        // Process channel
        samples[i] = processSample(samples[i], lfoSample);
    }
}

void Phaser::processStereo(float* const left, float* const right, const int numSamples) noexcept
{
    // Make sure left and right channels are not NULL
    jassert (left != nullptr && right != nullptr);
    
    float lfoSample;
    
    for(int i = 0; i < numSamples; i++) {
        // Get lfo sample
        LFO.waveForm = parameters.lfoWaveform;
        lfoSample = LFO.generateWaveSample();
        
        // process left and right channels
        left[i] = processSample(left[i], lfoSample);
        right[i] = processSample(right[i], lfoSample);
    }
}

inline float Phaser::processSample(float inSamp, float lfoSample)
{
    // Calculate sweep based on lfoSample and depth controls
    float d  = depthMin + (depthMax-depthMin) * (lfoSample + 1.f)/2.f;
    
    // Update the filter coefficients
    for( int i=0; i<6; i++ )
        allPass[i].delay( d );
    
    // Calculate the phased output
    float y = allPass[0].update(allPass[1].update(allPass[2].update(allPass[3].update(allPass[4].update(allPass[5].update(inSamp + zm1 * 0.1))))));
    zm1 = y;
    
    // mix dry and phaser output
    return inSamp*(1.0-parameters.mix/2) + y * parameters.depth * parameters.mix/2;
}

