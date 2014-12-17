//
//  LFOWaveformTable.h
//  NebuloMod
//
//  Created by Ryan Foo on 11/23/14.
//
//

#ifndef __NebuloMod__LFOWaveformTable__
#define __NebuloMod__LFOWaveformTable__

#include <stdio.h>
#include <math.h>
#include "DelayLine.h"
#include "WaveformComponent.h"

#define BUFFER_SIZE         1024
#define TABLE_SIZE          BUFFER_SIZE
#define SAMPLE_RATE         44100


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
    
    // Fills the LFO table buffer from the global table
    void fillLFOTable(float *table);
    
    /* Calculate the next value to come out of the wavetable buffer */
    float nextOut();
    
    /* increment to the next point in the table (not necessarily a set index */
    void setIncrement(double increment);
    
    void prepareToPlay();
    
    /* Generates a wave sample of the specified type */
    
    float generateWaveSample();
    // Stock Waveform Generation
    float generateSine(float freq);
    float generateTriangle(float freq);
    float generateSawtooth(float freq);
    float generateSquare(float freq);
    
    /* Looks up the lfotable value */
    float tableLookup();
    
    /* Generates the lfotable waveform */
    float generateLFOTable(float freq);
    
    float frequency;
    float waveForm;
    
    void setSampleRate(double sampleRate);
    
private:
    double currentSampleRate;
    float tableBuf[TABLE_SIZE];
    
    
};

#endif /* defined(__NebuloMod__LFOWaveformTable__) */
