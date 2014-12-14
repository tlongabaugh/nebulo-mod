
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
#include "LFOWaveformTable.h"

#define SAMPLE_RATE 44100
#define BOTTOM_FREQ 100

//------------------------------------------------------------------------
class Phaser
{
public:
    Phaser();
    ~Phaser();
    
    /* Holds the parameters used by the phaser */
    struct Parameters
    {
        Parameters() noexcept
        : depth(1.f),
        rate(5.f),
        lfoWaveform(0),
        mix(0.5f)
        {}
        
        float depth;
        float rate;
        int lfoWaveform;
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
    
    /* set the phaser feedback */
    //void setFeedback( float fb);
    
    /* set the phaser depth */
    //void setDepth(float newDepth);
    
    /* set the lfo rate */
    //void setRate(float newRate);
    
    /* set the lfo waveform */
   // void setWaveform(int newWaveform);

    //==============================================================================
    /** Applies effect to two stereo channels of audio data. */
    void processStereo (float* const left, float* const right, const int numSamples) noexcept;
    
    /** Applies effect single mono channel of audio data. */
    void processMono (float* const samples, const int numSamples) noexcept;
    
    /* applies phaser to one sample */
    float processSample(float inSamp, float lfoSample);

protected:
    void range(float fMin, float fMax);
    
private:
    // A simple AllPass class from www.musicdsp.org/files/phaser.cpp
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
    
    float depthMin, depthMax; //range
    float zm1;
};

#endif /* defined(__NebuloMod__Phaser__) */
