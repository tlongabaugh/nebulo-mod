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
void LFOTable::initTableBuffer(int *buf)
{
    
}

// Initialize our waveform graph/table
void LFOTable::initGraph(int *graph)
{
    
}

// Move test points to draw waveform; returns bool if changed;
// IF true, redraw graph and refill buffer
// O/W, don't redraw/don't refill buffer
bool LFOTable::movePointOne(int point)
{
    // default return
    return false;
}
bool LFOTable::movePointTwo(int point)
{
    // default return
    return false;
}

// Manipulate Graph (uses table buffer to draw graph)
void LFOTable::drawGraph(int *graph)
{
    
}

// Redraw graph when needed (callback everytime a change is detected/maybe only need drawGraph)
void LFOTable::redrawGraph(int *graph)
{
    
}