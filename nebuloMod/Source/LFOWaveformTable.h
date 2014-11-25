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

#define BUFFER_SIZE         2048

class LFOTable
{
public:
    // Initialize our table buffer
    void initTableBuffer(int *buf);
    // Initialize our waveform graph/table
    void initGraph(int *graph);
    
    // Move test points to draw waveform; returns bool if changed;
    // IF true, redraw graph and refill buffer
    // O/W, don't redraw/don't refill buffer
    bool movePointOne(int point);
    bool movePointTwo(int point);
    // Manipulate Graph (uses table buffer to draw graph)
    void drawGraph(int *graph);
    // Redraw graph when needed (callback everytime a change is detected/maybe only need drawGraph)
    void redrawGraph(int *graph);
};

#endif /* defined(__NebuloMod__LFOWaveformTable__) */
