
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
    
    
    //==============================================================================
    /** Applies effect to two stereo channels of audio data. */
    void processStereo (float* const left, float* const right, const int numSamples) noexcept;
    
    /** Applies effect single mono channel of audio data. */
    void processMono (float* const samples, const int numSamples) noexcept;
    
    /* Processes a frame of audio */
    float processAudioFrame(float inputSample, float lfoSample, int numChannels);

protected:
    void setDepth(float newDepth);
    void setMix(float mix);

    
private:    
    BiquadFilter leftAPF_1;
    BiquadFilter rightAPF_1;
    
    BiquadFilter leftAPF_2;
    BiquadFilter rightAPF_2;
    
    BiquadFilter leftAPF_3;
    BiquadFilter rightAPF_3;

    
    /*// Biquad APF filters to be cascaded
    BiQuad leftAPF_1;
    BiQuad rightAPF_1;

    BiQuad leftAPF_2;
    BiQuad rightAPF_2;
    
    BiQuad leftAPF_3;
    BiQuad rightAPF_3;*/
    
    // Holds phaser's parameters
    Parameters parameters;
    
    // min/max vars
    float minFreqAPF_1;
    float maxFreqAPF_1;
    
    float minFreqAPF_2;
    float maxFreqAPF_2;
    
    float minFreqAPF_3;
    float maxFreqAPF_3;
    
    //LFOtest LFO;
    
    LFOWaveformTable LFO;
    
    double currentSampleRate;
    
    /*
    //A Simple Allpass Delay from From www.musicdsp.org/files/phaser.cpp
    class AllpassDelay {
    public:
        AllpassDelay() : a1( 0.f ), zm1( 0.f )
        {}
        
        void delayAmt(float delay)
        { //sample delay time
            a1 = (1.f - delay) / (1.f + delay);
        }
        
        float update(float newSample)
        {
            float y = newSample * -a1 + zm1;
            zm1 = y * a1 + newSample;
            
            return y;
        }
    private:
        float a1, zm1;
    };
    
    class AllPassFilter
    {
    public:
        AllPassFilter() noexcept  : bufferSize (0), bufferIndex (0) {}
        
        void setSize (const int size)
        {
            if (size != bufferSize)
            {
                bufferIndex = 0;
                buffer.malloc ((size_t) size);
                bufferSize = size;
            }
            
            clear();
        }
        
        void clear() noexcept
        {
            buffer.clear ((size_t) bufferSize);
        }
        
        inline float process (const float input) noexcept
        {
            const float bufferedValue = buffer [bufferIndex];
            float temp = input + (bufferedValue * 0.5f);
            JUCE_UNDENORMALISE (temp);
            buffer [bufferIndex] = temp;
            bufferIndex = (bufferIndex + 1) % bufferSize;
            return bufferedValue - input;
        }
        
    private:
        HeapBlock<float> buffer;
        int bufferSize, bufferIndex;
        
        JUCE_DECLARE_NON_COPYABLE (AllPassFilter)
    };
    
    enum {numChannels = 2, numAllPasses = 4};
    
    //AllPassFilter allPass[numChannels][numAllPasses];
    
    AllpassDelay allPass[4];
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
 */
};

#endif /* defined(__NebuloMod__Phaser__) */
