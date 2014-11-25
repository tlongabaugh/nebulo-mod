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

// Initialize our buffer to write samples from table buffer
void LFOWaveform::initLFOBuffer(void)
{
    
}

// Write to buffer
bool LFOWaveform::writeBuffer(int *buf)
{
    // default return
    return false;
}

// Translate into a usable waveform
bool LFOWaveform::transformLFO(int *buf)
{
    
}