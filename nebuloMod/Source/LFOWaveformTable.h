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
#define TABLE_SIZE          BUFFER_SIZE
#define SAMPLE_RATE         44100

// LFO Data
typedef struct {
    float frequency;
} lfoData;

// Table Data
typedef struct {
    int pointOne;
    int pointTwo;
} tableData;;

class LFOTable
{
public:
    // Initialize our table buffer
    void initTableBuffer(int *buf, lfoData *userData);
    // Initialize our waveform graph/table
    void initGraph(int *graph);
    
    // Move test points to draw waveform; returns bool if changed;
    // IF true, redraw graph and refill buffer
    // O/W, don't redraw/don't refill buffer
    bool movePointOne(int point, tableData *userData);
    bool movePointTwo(int point, tableData *userData);
    // Manipulate Graph (uses table buffer to draw graph)
    void drawGraph(void);
    // Redraw graph when needed (callback everytime a change is detected/maybe only need drawGraph)
    void redrawGraph(int pointOne, int pointTwo, tableData *userData);
    
    // Stock Waveform Generation
    float generateSine(lfoData *userData);
    float generateTriangle(lfoData *userData);
    float generateSawtooth(lfoData *userData);
    float generateSquare(lfoData *userData);
}

#endif /* defined(__NebuloMod__LFOWaveformTable__) */
