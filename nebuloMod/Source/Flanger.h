//
//  Flanger.h
//  NebuloMod
//
//  Written by Tom Longabaugh
//
//

#ifndef __NebuloMod__Flanger__
#define __NebuloMod__Flanger__

#include "DelayLine.h"
#include "LFOWaveformTable.h"
#include "../JuceLibraryCode/JuceHeader.h"

#define INIT_SAMPLE_RATE    44100


//---------------------------------------------------------------------------------------------
class Flanger
{
public:
    Flanger ();
    ~Flanger ();

    /* Struct to hold the flanger parameters */
    typedef struct Parameters
    {
        Parameters() noexcept
        : depth(0.75f),
        rate(0.2f),
        lfoWaveform(0),
        feedback(0.2f),
        mix(0.5f)
        {}
        
        float depth;
        float rate;
        int lfoWaveform;
        float feedback;
        float mix;
    } Parameters;
    
    /* Returns the flangers parameters */
    const Parameters& getParameters(void) const noexcept
    {
        return parameters;
    }
    
    /* sets the flanger's parameters */
    void setParameters(const Parameters& newParam);
    
    /* Sets the sample rate of the flanger. Needs to be
       before any processing is done. */
    void setSampleRate (const double sampleRate);
    
    /* Initialize the LFO and clear the flanger delay line */
    void prepareToPlay();
    
    /* Process a mono set of samples */
    void processMono(float* const samples, const int numSamples);
    
    /* Process a stereo set of samples */
    void processStereo(float* const left, float* const right, const int numSamples);

private:
    Parameters parameters;      // Flanger parameters
    DelayLine delayLineL;       // Pointer to linear-interpolated delay line
    DelayLine delayLineR;
    LFOWaveformTable LFO;       // LFO for the flanger
    double currentSampleRate;   // Current sample rate of Flanger
    double _maxFlanging;        // Max flanging time (init to 20ms)
    
};

#endif /* defined(__NebuloMod__Flanger__) */
