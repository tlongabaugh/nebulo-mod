
//
//  Phaser.h
//  NebuloMod
//
//  Written by Tom Longabaugh with help from www.musicdsp.org
//

#ifndef __NebuloMod__Phaser__
#define __NebuloMod__Phaser__

#include "../JuceLibraryCode/JuceHeader.h"
#include "LFOWaveformTable.h"

#define INIT_SAMPLE_RATE 44100

//------------------------------------------------------------------------
class Phaser
{
public:
    Phaser();
    ~Phaser();
    
    /* Struct that holds the parameters used by the phaser */
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
    
    //============================================================================
    /** Applies effect single mono channel of audio data. */
    void processMono (float* const samples, const int numSamples) noexcept;
    
    /** Applies effect to two stereo channels of audio data. */
    void processStereo (float* const left, float* const right, const int numSamples) noexcept;
    
    /* applies phaser to one sample */
    float processSampleL(float inSamp, float lfoSample);
    
    /* applies phaser to one sample */
    float processSampleR(float inSamp, float lfoSample);

protected:
    /* sets the range of the phasers depth */
    void range(float fMin, float fMax);
    
private:
    /* A simple AllPass filter class built from www.musicdsp.org/files/phaser.cpp */
    class AllpassDelay{
    public:
        AllpassDelay()
        : _a1( 0.f )
        , _zm1( 0.f )
        {}
        
        /* sets the delay time */
        void delay(float delay){
            _a1 = (1.f - delay) / (1.f + delay);
        }
        
        /* processes a sample */
        float update(float inSamp){
            float y = inSamp * -_a1 + _zm1;
            _zm1 = y * _a1 + inSamp;
            
            return y;
        }
    private:
        float _a1, _zm1;
    };
    
    
    AllpassDelay allPassL[6];   // Six all pass filters to be cascaded for the phaser
    AllpassDelay allPassR[6];   // Six all pass filters for right channel
    Parameters parameters;      // Holds phaser's parameters
    LFOWaveformTable LFO;       // The LFO for the phaser
    double currentSampleRate;   // The sample rate to run the phaser at
    float depthMin, depthMax;   // Range for the phaser
    float zm1;                  // coeff
};

#endif /* defined(__NebuloMod__Phaser__) */
