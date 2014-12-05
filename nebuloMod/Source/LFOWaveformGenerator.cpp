//
//  LFOWaveformGenerator.cpp
//  NebuloMod
//
//  Created by Ryan Foo on 11/23/14.
//
//

/* 
 * SAMPLING of the LFO Waveform Table
 * 1) Takes samples from the table
 * 2) Write samples into our buffer
 * 3) Buffer is sent into LFO of Flanger/Phaser aspects of code
 */

#include "LFOWaveformGenerator.h"
// #include "LFOWaveformTable.h"

// Initialize our buffer to write samples from table buffer
void LFOWaveform::initLFOBuffer(void)
{
    int i;

    // Initialize Buffer to 0
    for (i = 0; i < BUFFER_SIZE; i++)
    {
        lfoBuf[i] = 0;
    }
}

// Write to buffer
bool LFOWaveform::writeBuffer(void)
{
    int i;
    
    // Write to buffer
    for (i = 0; i < BUFFER_SIZE; i++)
    {
        //lfoBuf[i] = tableBuf[i];
    
        return true;
    }
    
    // default return
    return false;
}

// Translate into a usable waveform
bool LFOWaveform::transformLFO(int *buf)
{
    // default return
    return false;
}