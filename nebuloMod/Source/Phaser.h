
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

#define SAMPLE_RATE 44100
#define BOTTOM_FREQ 100
#define PIN(n,min,max) ((n) > (max) ? max : ((n) < (min) ? (min) : (n)))


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
    
    /* Holds the parameters used by the */
    struct Parameters
    {
        Parameters() noexcept
        : depth(1.0f),
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
    
    /* Clear the phaser buffers */
    void reset();
    
    void setRange(float freqMin, float freqMax );
    
    void setRate(float rate);
    
    //==============================================================================
    /** Applies effect to two stereo channels of audio data. */
    void processStereo (float* const left, float* const right, const int numSamples) noexcept;
    
    /** Applies effect single mono channel of audio data. */
    void processMono (float* const samples, const int numSamples) noexcept;
    
    /* Processes one sample */
    float processSingleSample (float newSample) noexcept;

protected:
    void setDepth(float newDepth);
    void setMix(float mix);
/*
    float _paramSweepRate;		// 0.0-1.0 passed in
    float _paramWidth;			// ditto
    float _paramFeedback;		// ditto
    float _paramStages;			// ditto
    float _paramMixMode;		// ditto
    double _sweepRate;			// actual calc'd sweep rate
    double _width;				// 0-100%
    double _feedback;			// 0.0 to 1.0
    double _feedbackPhase;		// -1.0 or 1.0
    int   _stages;				// calc'd # of stages 2-10
    int	  _mixMode;				// mapped to supported mix modes
    
    double _wp;					// freq param for equation
    double _minwp;
    double _maxwp;
    double _sweepFactor;		// amount to multiply the freq by with each sample
    
    // the all pass line
    double _lx1;
    double _ly1;
    double _lx2;
    double _ly2;
    double _lx3;
    double _ly3;
    double _lx4;
    double _ly4;
    double _lx5;
    double _ly5;
    double _lx6;
    double _ly6;
    double _lx7;
    double _ly7;
    double _lx8;
    double _ly8;
    double _lx9;
    double _ly9;
    double _lx10;
    double _ly10;
    
    // output mixing
    double _mixLeftWet;
    double _mixLeftDry;
    double _mixRightWet;
    double _mixRightDry;*/
    
private:

    //A Simple Allpass Delay from From www.musicdsp.org/files/phaser.cpp
    class AllpassDelay {
    public:
        AllpassDelay() : a1( 0.f ), zm1( 0.f )
        {}
        
        void delayAmt(float delay){ //sample delay time
            a1 = (1.f - delay) / (1.f + delay);
        }
        
        float update(float newSample){
            float y = newSample * - a1 + zm1;
            zm1 = y * a1 + newSample;
            
            return y;
        }
    private:
        float a1, zm1;
    };
    
    AllpassDelay allPass[6];
    Parameters parameters;
    double currentSampleRate;

    float depthMin;
    float depthMax; //range
    //float feedBack; //feedback
    float lfoPhase;
    float lfoInc;
    float depth;
    
    float zm1;
    
    
    //JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Phaser);
};

#endif /* defined(__NebuloMod__Phaser__) */
