//
//  Flanger.h
//  NebuloMod
//
//  Created by Ryan Foo on 11/23/14.
//
//

#ifndef __NebuloMod__Flanger__
#define __NebuloMod__Flanger__

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "../JuceLibraryCode/JuceHeader.h"

// Global Definitions
#define SAMPLE_RATE         44100
#define INIT_LFO_FREQ       40
// BSZ is 1/5 of a second at 44100 Hz sample rate/must be power of 2
#define BSZ                 8192
#define ROUND(n)            ((int)((double)(n) + 0.5))
#define PIN(n, min, max)    ((n) > (max) ? max : ((n) < (min) ? (min) : (n)))
#define MODF(n, i, f)       ((i) = (int)(n), (f) = (n) - (double)(i))

// Two Pole Low Pass Filter Needed for Flanger Effect
class TwoPoleLowPassFilter
{
// LPF variables
private:
    double x1, x2, y1, y2;
    double a0, a1, a2, b0, b1, b2, w0, alpha;
    
// Constructor
public:
    TwoPoleLowPassFilter(float cutoff);
    ~TwoPoleLowPassFilter() {}
    double Process(double f);
};

class Flanger
{
public:
    // Constructor
    Flanger();              // put parameters in here for the constructor
    ~Flanger();
    
    // Global Sound Properties
    double *input_buffer;
    double *output_buffer;
    
    // Flanger Parameters
    enum Params
    {
        depth,
        rate,
        lfowaveform,
        feedback,
        mix,
    };
    
    // Hold the Parameters
    typedef struct Parameters
    {
        Parameters() noexcept
        : depth(0.5f),
          rate(0.5f),
          lfoWaveform(0),
          feedback(50.0f),
          mix(0.5f)
        {}
        
        float depth;
        float rate;
        // float lfo;
        int lfoWaveform;
        float feedback;
        // float manControl;
        float mix;
    } Parameters;

    void processMonoSamples(float* const samples, const int numSamples);
    void processStereoSamples(float* const left, float* const right, const int numSamples);
    Parameters& getParameters(void);
    void setParameters(const Parameters& newParam);
    void setSampleRate (const double sampleRate);
    void flush(void);
    
private:
    Parameters params;
    float gain, wet, dry;
    float min_dly, max_dly;
    int n = 0;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Flanger);
    
    
protected:
    // Control Filtering
    TwoPoleLowPassFilter *_delayControlFilter;
    TwoPoleLowPassFilter *_mixControlFilter;
};

#endif /* defined(__NebuloMod__Flanger__) */
