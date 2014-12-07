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

// Global Definitions
#define SAMPLE_RATE         44100
#define INIT_LFO_FREQ       40
// BSZ is 1/5 of a second at 44100 Hz sample rate/must be power of 2
#define BSZ                 8192
#define ROUND(n)            ((int)((double)(n) + 0.5))
#define PIN(n, min, max)    ((n) > (max) ? max : ((n) < (min) ? (min) : (n)))
#define MODF(n, i, f)       ((i) = (int)(n), (f) = (n) - (double)(i))

// Define the Globals, Parameters, and Variables needed for the Flanger
enum
{
    // Parameter Tags:
    kDepth,
    kRate = 0,
    kLFOWaveform,
    kResonance,
    kManualControl,
    kMix,
    
    kNumParams = 6,
    kNumMixModes = 6,
};

// Mix Modes to enable Mono/Stereo outputs
enum
{
    kMixMono,
    kMixMonoMinus,
    kMixMonoBoth,
    kMixStereo,
    kMixStereoMinus,
    kMixStereoBoth,
};

// For future development for presets
/*
class FlangerEffect
{
    friend class Flanger;
// Flanger constructor
public:
    FlangerEffect();
    
    FlangerEffect(float depth,
                  float rate,
                  float lfo,
                  float resonance,
                  float manControl,
                  float mix,
                  float mixMode);
    ~FlangerEffect() {}
    
// Flanger variables
private:
    float paramDepth;
    float paramRate;
    float paramLFO;
    float paramResonance;
    float paramManCtrl;
    float paramMix;
    float paramMixMode;
};
*/

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
    // Constructor
    Flanger();              // put parameters in here for the constructor
    ~Flanger();
    
protected:
    // Flanger Methods
    void setDepth(float depth);
    void setRate(float rate);
    void setLFO(float lfo, float waveform);
    void setResonance(float resonance);
    void setManualControl(float manCtrl);
    void setMix(float mix);
    void setMixMode(float mode);
    
    // Parameters (Scaled)
    float paramDepth;
    float paramRate;
    float paramLFO;
    float paramLFOWaveform;
    float paramResonance;
    float paramManCtrl;
    float paramMix;
    float paramMixMode;
    
    // Actual Values (not scaled)
    double depth;
    double rate;
    double lfo;
    double lfoWaveform;
    double resonance;
    double manCtrl;
    double mix;
    double mixMode;
    
    // For sound
    double *input_buffer;
    double *output_buffer;
    
    // Output Mixing
    double _mixLeftWet;
    double _mixLeftDry;
    double _mixRightWet;
    double _mixRightDry;
    bool _mixMono;
    
    // Control Filtering
    TwoPoleLowPassFilter *_delayControlFilter;
    TwoPoleLowPassFilter *_mixControlFilter;
};

#endif /* defined(__NebuloMod__Flanger__) */
