
//
//  Phaser.h
//  NebuloMod
//
//  Created by Ryan Foo on 11/23/14.
//
//

#ifndef __NebuloMod__Phaser__
#define __NebuloMod__Phaser__

#include <stdio.h>
#include <math.h>
#include "../JuceLibraryCode/JuceHeader.h"
#include "Biquad.h"
#include "LFOWaveformTable.h"
#include "LFOtest.h"
#include "dRowAudio_BiquadFilter.h"

#define SAMPLE_RATE 44100
#define BOTTOM_FREQ 100
//#define PIN(n,min,max) ((n) > (max) ? max : ((n) < (min) ? (min) : (n)))


//------------------------------------------------------------------------
class Phaser
{
public:
    Phaser();
    ~Phaser();
    
    // Parameter enums
    enum
    {
        kDepth,
        kRate,
        kLfowaveform,
        kFeedback,
        kMix,
    };
    
    /* Holds the parameters used by the phaser */
    struct Parameters
    {
        Parameters() noexcept
        : depth(1.0f),
        rate(0.5f),
        mix(1.0f)
        {}
        
        float depth;
        float rate;
        // float lfo;
        int lfoWaveform;
        // float manControl;
        float mix;
    };
    
    /* Returns the parameters for the phaser */
    const Parameters& getParameters() const noexcept
    {
        return parameters;
    };
    
    /* Apply new set of parameters to the phaser */
    void setParameters (const Parameters& newParams);
    
    /* Sets the sample rate that the phaser will use. This needs to be
     called before the process method */
    void setSampleRate(const double sampleRate);
    
    /* Clear the phaser buffers and set up lfo*/
    void prepareToPlay();
    
    // calculates the new filter cutoff from the envelope value
    float calculateAPFCutoffFreq(float LFOsample, float minFreq, float maxFreq);
    
    // Calculate BiQuad coeffs (APF)
    void calculateFirstOrderAPFCoeffsLeft(float LFOsample);
    void calculateFirstOrderAPFCoeffsRight(float LFOsample);
    
    // Helper function for APF calculation
    void calculateFirstOrderAPFCoeffs(float cutoffFreq, BiQuad* BiQuadFilter);
    
    //float processSample(float inSamp);
    
    //==============================================================================
    /** Applies effect to two stereo channels of audio data. */
    void processStereo (float* const left, float* const right, const int numSamples) noexcept;
    
    /** Applies effect single mono channel of audio data. */
    void processMono (float* const samples, const int numSamples) noexcept;
    
    /* applies phaser to one sample */
    float processSample(float inSamp, float lfoSample);
    
    void Range(float fMin, float fMax);
    void Feedback( float fb);
    
    void Depth(float depth);

protected:
    void setDepth(float newDepth);
    void setMix(float mix);

    
private:
    class AllpassDelay{
    public:
        AllpassDelay()
        : _a1( 0.f )
        , _zm1( 0.f )
        {}
        
        void delay(float delay){ //sample delay time
            _a1 = (1.f - delay) / (1.f + delay);
        }
        
        float update(float inSamp){
            float y = inSamp * -_a1 + _zm1;
            _zm1 = y * _a1 + inSamp;
            
            return y;
        }
    private:
        float _a1, _zm1;
    };
    
    AllpassDelay allPass[6];
    
    // Holds phaser's parameters
    Parameters parameters;
    
    LFOWaveformTable LFO;
    
    double currentSampleRate;
    
    float _dmin, _dmax; //range
    float _fb; //feedback
    float _depth;
    float _zm1;
};

#endif /* defined(__NebuloMod__Phaser__) */
