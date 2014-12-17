//
//  CurvePoint.h
//  NebuloMod
//
//  Created by Ryan Foo on 12/16/14.
//
//  Original file created by David Rowland (Look at drowAudio github)
/**
 * Gives our drawn points to have mouse listeners
 * We use this to allow us to use the points to move the waveform and customize it.
 */

#ifndef NebuloMod_CurvePoint_h
#define NebuloMod_CurvePoint_h

#include "../JuceLibraryCode/JuceHeader.h"

class CurvePoint  : public Component,
public ComponentDragger
{
private:
    //==============================================================================
    ComponentBoundsConstrainer constrainer;
    bool mouseIsOver;
    
public:
    //==============================================================================
    CurvePoint ()
    :	mouseIsOver (false)
    {
    }
    
    ~CurvePoint ()
    {
    }
    
    //==============================================================================
    void resized ()
    {
        int halfWidth = getWidth() / 2;
        int halfHeight = getHeight() / 2;
        constrainer.setMinimumOnscreenAmounts (halfHeight, halfWidth, halfHeight, halfWidth);
    }
    
    void paint (Graphics& g)
    {
        g.setColour (Colours::grey);
        g.fillAll();
        
        if (mouseIsOver)
        {
            g.setColour (Colours::lightgrey);
            g.drawRect (0, 0, getWidth(), getHeight(), 2);
        }
    }
    
    void mouseDown (const MouseEvent& e)
    {
        startDraggingComponent (this, e);
    }
    
    void mouseDrag (const MouseEvent& e)
    {
        dragComponent (this, e, &constrainer);
    }
    
    void mouseEnter (const MouseEvent& /*e*/)
    {
        mouseIsOver = true;
        repaint();
    }
    
    void mouseExit (const MouseEvent& /*e*/)
    {
        mouseIsOver = false;
        repaint();
    }
    
    //==============================================================================
};


#endif
