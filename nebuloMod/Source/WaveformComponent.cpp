//
//  WaveformComponent.cpp
//  NebuloMod
//
//  Created by Ryan Foo on 12/16/14.
//
//

#include "WaveformComponent.h"

WaveformComponent::WaveformComponent():isInitialised(false)
{
    for (int i = 0; i < 2; ++i)
    {
        curvePoints.add(new CurvePoint());
        curvePoints[i]->addComponentListener(this);
        addAndMakeVisible(curvePoints[i]);
    }
}

WaveformComponent::~WaveformComponent()
{
    for (int i = 0; i < 2; ++i)
    {
        curvePoints[i]->removeComponentListener(this);
    }
}

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
    g.drawLine (0.0f, (float) h, (float) w, 0.0f);
    
    refreshPath();
}

void WaveformComponent::paint (Graphics& g)
{
    g.drawImageAt (background, 0, 0);
    
    g.setColour (Colours::white);
    g.strokePath (path, PathStrokeType (2.0f));
}

void WaveformComponent::componentMovedOrResized (Component& component, bool /*wasMoved*/, bool /*wasResized*/)
{
    if (&component == curvePoints[0] || &component == curvePoints[1])
    {
        float x1 = (curvePoints[0]->getX() + (0.5f * curvePoints[0]->getWidth())) / (float) getWidth();
        float y1 = ((getHeight() - curvePoints[0]->getY()) - (0.5f * curvePoints[0]->getHeight())) / (float) getHeight();
        
        float x2 = (curvePoints[1]->getX() + (0.5f * curvePoints[1]->getWidth())) / (float) getWidth();
        float y2 = ((getHeight() - curvePoints[1]->getY()) - (0.5f * curvePoints[1]->getHeight())) / (float) getHeight();
        
        // refillBuffer (x1, y1, x2, y2);
    }
}

void WaveformComponent::refreshPath()
{
    //const int bufferSize = buffer.getSize();
    const int w = getWidth();
    const int h = getHeight();
    
    const float xScale = (float) w / (float) 1024;
    const float yScale = (float) h;
    
    path.clear();
    path.startNewSubPath (0.0f, (float) h);
    
    for (int i = 0; i < 1024; ++i)
    {
        path.lineTo (i * xScale, h - (waveformTable[i] * yScale));
    }
    
    repaint();
}
/*
void WaveformComponent::refillBuffer (float x1, float y1, float x2, float y2)
{
    float* bufferData = buffer.getData();
    const int bufferSize = buffer.getSize();
    const float bufferScale = 1.0f / (float) bufferSize;
    
    for (int i = 0; i < bufferSize; ++i)
    {
        float x = jlimit (0.0f, 1.0f, i * bufferScale);
        bufferData[i] = BezierCurve::cubicBezierNearlyThroughTwoPoints (x,
                                                                        x1, y1,
                                                                        x2, y2);
    }
    
    buffer.updateListeners();
}
*/
/*
void WaveformComponent::resetBuffer()
{
    float* bufferData = buffer.getData();
    const int bufferSize = buffer.getSize();
    const float bufferScale = 1.0f / bufferSize;
    
    for (int i = 0; i < bufferSize; ++i)
        bufferData[i] = bufferScale * i;
    
    resetPoints();
    buffer.updateListeners();
}
 */

void WaveformComponent::resetPoints()
{
    const int w = getWidth();
    const int h = getHeight();
    
    const int bufferSize = 1024;
    const float* bufferData = waveformTable;
    
    float x1 = w * 0.25f;
    // float y1 = h * linearInterpolate (bufferData, bufferSize, bufferSize * 0.75f);
    
    float x2 = w * 0.75f;
    // float y2 = h * linearInterpolate (bufferData, bufferSize, bufferSize * 0.25f);
    
    //curvePoints[0]->setBounds ((int) (x1 - 5), (int) (y1 - 5), 10, 10);
    //curvePoints[1]->setBounds ((int) (x2 - 5), (int) (y2 - 5), 10, 10);
}