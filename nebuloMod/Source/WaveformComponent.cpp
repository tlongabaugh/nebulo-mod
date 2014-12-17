//
//  WaveformComponent.cpp
//  NebuloMod
//
//  Created by Ryan Foo on 12/16/14.
//
//

#include "WaveformComponent.h"

// Called from extern waveformTable in LookupTable.h
volatile float waveformTable[1024];

// Construtor
WaveformComponent::WaveformComponent():isInitialised(false)
{
    // Initialize our waveform points
    for (int i = 0; i < 3; ++i)
    {
        curvePoints.add(new CurvePoint());
        curvePoints[i]->addComponentListener(this);
        addAndMakeVisible(curvePoints[i]);
    }
    
    // Set flags
    secondTime = false;
    initBuffer = true;
    
    // Initialize our table values to default sine wave lookup table values
    for (int i = 0; i < 1024; i++)
    {
        waveformTable[i] = defaults[i];
    }
}

// Deconstrutor
WaveformComponent::~WaveformComponent()
{
    // remove component listeners from our waveform points
    for (int i = 0; i < 2; ++i)
    {
        curvePoints[i]->removeComponentListener(this);
    }
}

// When something in the component is resized, redraw graphics and refresh line path graph
void WaveformComponent::resized()
{
    const int w = getWidth();
    const int h = getHeight();
    
    if ((!isInitialised) && (w > 0) && (h > 0))
    {
        resetPoints();
        isInitialised = true;
    }
    
    background = Image(Image::RGB, jmax(1,w), jmax(1,h), false);
    Graphics g (background);
    g.fillAll(Colours::black);
    
    g.setColour(Colour::greyLevel(0.25f));
    
    const float xScale = w / 10.0f;
    const float yScale = h / 10.0f;
    
    for (int i = 1; i < 10; ++i)
    {
        g.drawHorizontalLine((int) (i * yScale), 0.0f, (float) w);
        g.drawVerticalLine((int) (i * xScale), 0.0f, (float) h);
    }
    
    refreshPath(0);
}

// Paint the line path graph
void WaveformComponent::paint (Graphics& g)
{
    g.drawImageAt (background, 0, 0);
    
    g.setColour (Colours::white);
    g.strokePath (path, PathStrokeType (3.0f));
}

// Whenever a point is moved in the GUI, we take those points and refill our buffer to:
// 1. Redraw the graph
// 2. Put new values in the waveformTable
void WaveformComponent::componentMovedOrResized (Component& component, bool wasMoved, bool wasResized)
{
    // If point 1 and 3 are moved
    if (&component == curvePoints[0] || &component == curvePoints[2])
    {
        float x1 = (curvePoints[0]->getX() + (0.5f * curvePoints[0]->getWidth())) / (float) getWidth();
        float y1 = ((getHeight() - curvePoints[0]->getY()) - (0.5f * curvePoints[0]->getHeight())) / (float) getHeight();
        
        float x2 = (curvePoints[1]->getX() + (0.5f * curvePoints[1]->getWidth())) / (float) getWidth();;
        float y2 = ((getHeight() - curvePoints[1]->getY()) - (0.5f * curvePoints[1]->getHeight())) / (float) getHeight();;
        
        float x3 = (curvePoints[2]->getX() + (0.5f * curvePoints[2]->getWidth())) / (float) getWidth();
        float y3 = ((getHeight() - curvePoints[2]->getY()) - (0.5f * curvePoints[2]->getHeight())) / (float) getHeight();
        
        // Refill Buffer
        refillBuffer (x1, y1, x2, y2, x3, y3);
    }
}

// Draws our line path graph
void WaveformComponent::refreshPath(int lfo_wave)
{
    const int w = getWidth();
    const int h = getHeight();
    
    const float xScale = (float) w / (float) 1024;
    const float yScale = (float) h/ (float) 2.05;
    
    // Clear original path
    path.clear();
    
    // If sine, tri, or custom wave, start from 0 position
    if (lfo_wave == 0 || lfo_wave == 1 || lfo_wave == 4)
    {
        path.startNewSubPath (0.0f, (float) h/2);
    }
    // If sawtooth, start from top position
    else if (lfo_wave == 2)
    {
        path.startNewSubPath (0.0f, 5.0f);
    }
    // If square, start from bottom position
    else if (lfo_wave == 3)
    {
        path.startNewSubPath(0.0f, 205.0f);
    }
    
    // Draw the line!
    for (int i = 0; i < 1024; ++i)
    {
        // Sine wave
        if (lfo_wave == 0)
        {
            path.lineTo(i * xScale, (h/2) - (defaults[i-1] * yScale));
        }
        // Triangle wave
        else if (lfo_wave == 1)
        {
            if (i < 256)
                path.lineTo(i * xScale, i * -yScale/255 + h/2);
            else if (i < 768)
                path.lineTo(i * xScale, i * yScale/256 - 100);
            else if (i < 1024)
                path.lineTo(i * xScale, i * -yScale/255 + 515);
        }
        // Sawtooth wave
        else if (lfo_wave == 2)
        {
            if (i < 15)
                path.lineTo(i * xScale, 5);
            else if (i < 1010)
                path.lineTo(i * xScale, i * -yScale/530 + 205);
            else if (i < 1024)
                path.lineTo(i * xScale, 205);
        }
        // Square Wave
        else if (lfo_wave == 3)
        {
            if (i < 25)
                path.lineTo(i * xScale, 205);
            else if (i < 512)
                path.lineTo(i * xScale, 5);
            else if (i < 999)
                path.lineTo(i * xScale, 205);
            else if ( i < 1024)
                path.lineTo(i * xScale, 5);
        }
        // Custom wave
        else if (lfo_wave == 4)
        {
            path.lineTo(i * xScale, (h/2) - (waveformTable[i-1] * yScale));
        }
    }
    
    // Repaint the line path graph once we have drawn out the path
    repaint();
}

// Enable/disable points/visibility depending on which lfo we are using
void WaveformComponent::enablePoints(bool isEnabled)
{
    curvePoints[0]->setEnabled(isEnabled);
    curvePoints[2]->setEnabled(isEnabled);
    
    curvePoints[0]->setVisible(isEnabled);
    curvePoints[2]->setVisible(isEnabled);
}

// Refill buffer scales new points in the GUI and waveformTable
// We have three points here: (x1,y1) [first marker], (x2,y2) [midpoint], and (x3,y3) [second marker].
// These points are used to redraw the graph.
// The bezier curve is applied to the first marker and midpoint and another curve is applied to the midpoint and second marker.
void WaveformComponent::refillBuffer (float x1, float y1, float x2, float y2, float x3, float y3)
{
    // Determined by size of buffer (1024) and increment (i = 1)
    const float bufferScale = 1.0f / (float) 1024;
    
    // If we are not initing the buffer, redraw
    if (!initBuffer && secondTime)
    {
        for (int i = 0; i < 1024; ++i)
        {
            // Limit to how far we can go
            float x = jlimit (-1.0f, 1.0f, i * bufferScale);
            // waveformTable[i] = BezierCurve::cubicBezierNearlyThroughTwoPoints (x, x1, y1, x2, y2);
            
            // SCALING HERE
            if (i < 512)
            {
                waveformTable[i] = BezierCurve::cubicBezierNearlyThroughTwoPoints(x, x1, y1, x2, y2);
            }
            else if (i < 1024)
            {
                waveformTable[i] = BezierCurve::cubicBezierNearlyThroughTwoPoints(x, x2, y2, x3, y3) - 1;
            }
        }
        
        if (initBuffer) {
            initBuffer = false;
        }
        else if (!secondTime) {
            secondTime = true;
        }
        
        // Redraw
        refreshPath(4);
    }
    else
    {
        if (!initBuffer)
            secondTime = true;
        
        initBuffer = false;
        refreshPath(0);
    }
}

// Reset the buffer with default values
void WaveformComponent::resetBuffer()
{
    // Set default values
    for (int i = 0; i < 1024; ++i)
    {
        waveformTable[i] = defaults[i];
    }
    
    // Set Flags
    secondTime = false;
    initBuffer = true;
    
    // Reset our Points
    resetPoints();
    // Redraw the line graph
    refreshPath(4);
}

// Reset points to default positions
void WaveformComponent::resetPoints()
{
    // Get the width and height of GUI
    const int w = getWidth();
    const int h = getHeight();
    
    // Default Point Values
    float x1 = w * 0.25f - 4.5;
    float y1 = (h/2) - waveformTable[240] * h/2.02;
    
    float x2 = w * 0.5f - 5;
    float y2 = h/2;
    
    float x3 = w * 0.75f - 5;
    float y3 = (h/2) - waveformTable[767] * h/2.21;
    
    // Set the curve points
    curvePoints[0]->setBounds ((int) (x1), (int) (y1), 10, 10);
    curvePoints[1]->setBounds ((int) (x2), (int) (y2), 10, 10);
    curvePoints[2]->setBounds ((int) (x3), (int) (y3), 10, 10);
    
    // Don't let curve point 1 be visible
    curvePoints[1]->setVisible(false);
}