//
//  LFOWaveformTable.h
//  NebuloMod
//
//  Created by Ryan Foo on 11/23/14.
//
//

#ifndef __NebuloMod__LFOWaveformTable__
#define __NebuloMod__LFOWaveformTable__

#include <math.h>
#include "../JuceLibraryCode/JuceHeader.h"
#include "DelayLine.h"
#include "LookupTable.h"

#define BUFFER_SIZE         1024
#define TABLE_SIZE          BUFFER_SIZE
#define INIT_SAMPLE_RATE         44100

// volatile float waveformTable[1024];

class LFOWaveformTable : public DelayLine
{
public:
    LFOWaveformTable();
    ~LFOWaveformTable();
    
    typedef enum {
        sineWave,
        triWave,
        sawWave,
        squareWave,
        tableWave
    } waveType;
    
    /* Calculate the next value to come out of the wavetable buffer */
    float nextOut();
    
    /* increment to the next point in the table (not necessarily a set index */
    void setIncrement(double increment);
    
    void prepareToPlay();
    
    /* Generates a wave sample of a specified type */
    float generateWaveSample();
    
    /* Specific waveform generation funcitons, called by generateWaveSample() */
    float generateSine(float freq);
    float generateTriangle(float freq);
    float generateSawtooth(float freq);
    float generateSquare(float freq);
    
    /* Looks up the lfotable value */
    float tableLookup();
    
    /* Returns the lfotable waveform value
     * This method increments over the buffer an equal amount of times related to 
       lfoTable[] array length over period.
     */
    float generateLFOTableSample(float freq);
    
    /* Frequency and waveform params (frequently set by other classes) */
    float frequency;
    float waveForm;
    
    /* Sets the sample rate that the lfo runs at (same as global sample rate */
    void setSampleRate(double sampleRate);
    
private:
    double currentSampleRate;   // The current sample rate
    IIRFilter lowpass;          // filter for smoothing LFOs
    IIRCoefficients coeffs;     // coefficients for setting lowpas filter
    
};

#endif /* defined(__NebuloMod__LFOWaveformTable__) */
