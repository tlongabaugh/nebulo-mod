//
//  LFOWaveformTable.cpp
//  NebuloMod
//
//  Created by Ryan Foo on 11/23/14.
//
//

/*
 * GUI ASPECT of the LFO Waveform
 * 1) Allows the user to draw a customizable waveform 
 * 2) Only 2 movable positions and 1 steady position
 * 3) Every time you move a position, store the sample into a buffer
 *
 * Make table buffer public so we can use it in other classes
 */

#include "LFOWaveformTable.h"

// Initialize our table buffer
void LFOTable::initTableBuffer(int *buf, lfoData *userData)
{
    // put sine waveform into buffer
    lfoData *data = userData;
    float sample, phase;
    static float prev_phase;
    int i;
    
    // Initialize sine waveform using data frequency and put into tableBuf
    for (i = 0; i < BUFFER_SIZE; i++)
    {
        phase = 2 * M_PI * data->frequency / SAMPLE_RATE + prev_phase;
        sample = sin(phase);
        
        tableBuf[i] = sample;
    }
    
}

// Initialize our waveform graph/table
void LFOTable::initGraph(int *graph)
{
    // Interact with GUI here
    int i;
    
    for (i = 0; i < TABLE_SIZE; i++)
    {
        // poistion 0
        tableBuf[i] = 0;
        // use point to draw one point on the graph
    }
}

// Move test points to draw waveform; returns bool if changed;
// IF true, redraw graph and refill buffer
// O/W, don't redraw/don't refill buffer
bool LFOTable::movePointOne(int point, tableData *userData)
{
    tableData *data = userData;
    
    // If we detect a move:
    //  Then draw something and then return true
    if (point != data->pointOne)
    {
        data->pointOne = point;
        
        // Set the point in tableBuf
        tableBuf[512] = data->pointOne;
        return true;
    }
    
    // default return
    return false;
}
bool LFOTable::movePointTwo(int point, tableData *userData)
{
    tableData *data = userData;
    
    // If we detect a move:
    //  Then draw something and then return true
    if (point != data->pointTwo)
    {
        data->pointTwo = point;
        
        // Set the point in tableBuf
        tableBuf[1536] = data->pointTwo;
        return true;
    }
    
    // default return
    return false;
}

// Manipulate Graph (uses table buffer to draw graph)
void LFOTable::drawGraph(void)
{
    // Use tableBuf info to draw the graph
    // Interact with GUI here
    int i;
    float point;
    
    for (i = 0; i < TABLE_SIZE; i++)
    {
        // Write tableBuf to position
        point = tableBuf[i];
        // use point to draw one point on the graph
    }
}

// Redraw graph when needed (callback everytime a change is detected/maybe only need drawGraph)
void LFOTable::redrawGraph(int pointOne, int pointTwo, tableData *userData)
{
    tableData *data = userData;

    // Move Points
    movePointOne(pointOne, data);
    movePointTwo(pointTwo, data);

    // Redraw the graph
    drawGraph();
}

// Stock Waveform Generation
float LFOTable::generateSine(lfoData *userData)
{
    float phase, sinSample = 0;
    static float prev_phase;
    lfoData *data = userData;
    
    phase = 2 * M_PI * data->frequency / SAMPLE_RATE + prev_phase;
    sinSample = sin(phase);
    
    if (phase > (2 * M_PI))
    {
        phase -= (2 * M_PI);
    }
    
    prev_phase = phase;
    
    return sinSample;
}

float LFOTable::generateTriangle(lfoData *userData)
{
    static float triSample;
    static float counter = 1;
    lfoData *data = userData;
    float T = SAMPLE_RATE / data->frequency;
    
    if (counter == 1)
    {
        triSample += 2./T;
    }
    else
    {
        triSample -= 2./T;
    }
    
    if (triSample >= 1)
    {
        counter = 0;
    }
    else if (triSample <= -1)
    {
        counter = 1;
    }
    
    return triSample;
}

float LFOTable::generateSawtooth(lfoData *userData)
{
    static float sawSample;
    lfoData *data = userData;
    float T = SAMPLE_RATE / data->frequency;
    
    sawSample += 2./T;
    
    if (sawSample >= 1)
    {
        sawSample -= 2;
    }
    
    return sawSample;
}

float LFOTable::generateSquare(lfoData *userData)
{
    float sqrSample, phase;
    static float prev_phase;
    lfoData *data = userData;
    
    phase = 2 * M_PI * data->frequency / SAMPLE_RATE + prev_phase;
    sqrSample = sin(phase);
    
    if (sqrSample > 0)
    {
        sqrSample = 1;
    }
    else
    {
        sqrSample = -1;
    }
    if (phase > (2 * M_PI))
    {
        phase -= (2 * M_PI);
    }
    
    prev_phase = phase;
    
    return sqrSample;
}