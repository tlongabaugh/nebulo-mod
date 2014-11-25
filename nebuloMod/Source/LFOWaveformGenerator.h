//
//  LFOWaveformGenerator.h
//  NebuloMod
//
//  Created by Ryan Foo on 11/23/14.
//
//

#ifndef __NebuloMod__LFOWaveformGenerator__
#define __NebuloMod__LFOWaveformGenerator__

#include <stdio.h>

#define BUFFER_SIZE         2048

// Global Variables
int lfoBuf[BUFFER_SIZE];

class LFOWaveform
{
public:
    // Initialize our buffer to write samples from table buffer
    void initLFOBuffer(void);
    // Write to buffer
    bool writeBuffer(void);
    // Translate into a usable waveform
    bool transformLFO(int *buf);
};

#endif /* defined(__NebuloMod__LFOWaveformGenerator__) */
