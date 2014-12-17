/*
  ==============================================================================

    This file was auto-generated by the Introjucer!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

// Written by Ryan Foo

#include "PluginEditor.h"

// Thanks Uri!!! So helpful man!
void NebuloModAudioProcessorEditor::createSlider(Slider &slider, Slider::SliderStyle style, double defaultVal, double min, double max, double incr, std::string name) {
    // these define the parameters of our slider object
    slider.setSliderStyle(style);
    slider.setRange(min, max, incr);
    slider.setTextBoxStyle (Slider::NoTextBox, false, 90, 0);
    slider.setPopupDisplayEnabled (true, this);
    slider.setTextValueSuffix(" " + name);
    slider.setValue(defaultVal);
    
    // set this class as the listener for the slider's callback function
    slider.addListener(this);
    
    // this function adds the slider to the editor
    addAndMakeVisible(&slider);
    slider.setVisible(true);
    
    slider.setComponentID(name);
    slider.isAlwaysOnTop();
}

void NebuloModAudioProcessorEditor::createLabel(Label &label, std::string name)
{
    label.setSize(100,50);
    label.setEnabled(true);
    label.setText(name, dontSendNotification);
    addAndMakeVisible(label);
    label.setVisible(true);
    label.isAlwaysOnTop();
}

//==============================================================================
NebuloModAudioProcessorEditor::NebuloModAudioProcessorEditor (NebuloModAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (700, 550);
    
    // EFFECT PARAMETER SIDE
    
    // Menu
    modMenu.setText("Select Modulation");
    modMenu.addItem("Flanger", 1);
    modMenu.addItem("Phaser", 2);
    modMenu.setEnabled(true);
    modMenu.setSize(180, 30);
    addAndMakeVisible(&modMenu);
    modMenu.addListener(this);
    
    // Create dah sliders!!!
    createSlider(depthSlider, Slider::LinearHorizontal, processor.flDepthVal, 0.0, 1.0, 0.01, "Depth");
    createSlider(rateSlider, Slider::LinearHorizontal, processor.flRateVal, .1, 5.0, 0.01, "Rate");
    createSlider(feedbackSlider, Slider::LinearHorizontal, processor.flFeedbackVal, 0.0, 100.0, 1.0, "Feedback");
    createSlider(mixSlider, Slider::LinearHorizontal, processor.flMixVal, 0.0, 1.0, 0.01, "Mix");
    
    // Create texts!
    createLabel(depthText, "Depth");
    createLabel(rateText, "Rate");
    createLabel(feedbackText, "Feedback");
    createLabel(mixText, "Mix");
    createLabel(debugText, "Debugger");
    
    fxText.setSize(200, 50);
    
    // LFO PARAMETER SIDE

    // Menu
    lfoMenu.setText("Select LFO Waveform");
    lfoMenu.addItem("Sine", 1);
    lfoMenu.addItem("Triangle", 2);
    lfoMenu.addItem("Sawtooth", 3);
    lfoMenu.addItem("Square", 4);
    lfoMenu.addItem("Custom", 5);
    lfoMenu.setSize(180, 30);
    addAndMakeVisible(&lfoMenu);
    lfoMenu.addListener(this);
    
    // Inits:
    modMenu.setSelectedItemIndex(0);
    lfoMenu.setSelectedItemIndex(0);
    LFO.waveForm = 0;
    feedbackSlider.setEnabled(true);
    fxText.setText("Flanger", dontSendNotification);
    processor.flLfoWaveformVal = LFO.waveForm;
    processor.phsLfoWaveformVal = LFO.waveForm;
    
    // Update Phaser
    processor.updateFlanger();
    
    // Debugging Text
    addAndMakeVisible(debugText);
    
    // Waveform Component (??)
    addAndMakeVisible(&wavComponent);
    
    // Flags
    processor.flanger_active = true;
    processor.phaser_active = false;
    initDrawing = true;
    isMoving_marker_one = false;
    isMoving_marker_two = false;
}

NebuloModAudioProcessorEditor::~NebuloModAudioProcessorEditor()
{
}

//==============================================================================
void NebuloModAudioProcessorEditor::paint (Graphics& g)
{
    // Background
    g.fillAll (Colours::khaki);
    
    // Texts and Line Graph
    g.setColour (Colours::black);
    g.setFont (28.0f);
    g.drawFittedText ("Nebulo Mod", getLocalBounds(), Justification::bottomLeft, 1);
    g.setFont(16.0f);
    g.drawFittedText("by Ryan Foo and Tom Longabaugh", getLocalBounds(), Justification::bottomRight, 2);
    
    // Backgrounds son!!!
    g.setColour(Colours::ivory);
    g.fillRoundedRectangle(320, 160, 370, 250, 25);
    g.fillRoundedRectangle(20, 160, 280, 250, 25);
    
    float lineThickness = 2.0f;
    
    /*
    // Wavetable graph init
    if (initDrawing)
    {
        waveRef.startNewSubPath(40, 280);
        for (int i = 0; i < 1024; i++)
        {
            // reaches to 270 by the end
            float x_axis = (i+1) * 15 / 64;
            static float init_y_axis;
            init_y_axis = our_sineTable[i] * -100;
            
            waveRef.lineTo(40 + x_axis, 280 + init_y_axis);
        }
    
        // The line graph!!
        g.setColour(Colours::black);
        g.strokePath(waveRef, PathStrokeType(lineThickness));
        initDrawing = false;
    }
    else
    {
        // Update the path graph
        updatePath();
        g.setColour(Colours::black);
        g.strokePath(waveRef, PathStrokeType(lineThickness));
    }
     */
}

void NebuloModAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    // Knobs!!!
    depthSlider.setBounds(350, 160, 250, 75);
    rateSlider.setBounds(350, 210, 250, 75);
    feedbackSlider.setBounds(350, 260, 250, 75);
    mixSlider.setBounds(350, 310, 250, 75);
    
    // Menus!!!
    modMenu.setBounds(390, 120, 200, 200);
    lfoMenu.setBounds(70, 120, 200, 200);
    
    // Texts!!!
    depthText.setBounds(610, 170, 25, 75);
    rateText.setBounds(610, 220, 25, 75);
    feedbackText.setBounds(610, 270, 25, 75);
    mixText.setBounds(610, 320, 25, 75);
    fxText.setBounds(450, 360, 100, 50);
    
    // Waveform Component
    wavComponent.setBounds(35, 180, 250, 210);          // 250 210
    
    debugText.setBounds(350, 400, 150, 150);
}

// Update our GUI when a slider is changed
// AUTOMATIC CALLBACK
void NebuloModAudioProcessorEditor::sliderValueChanged(Slider* slider)
{    
    // Update Flanger
    if (modMenu.getSelectedItemIndex() == 0)
    {
        if (slider->getComponentID().compare("Mix") == 0) {
            processor.flMixVal = slider->getValue();
        }
        else if (slider->getComponentID().compare("Depth") == 0) {
            processor.flDepthVal = slider->getValue();
            // debugText.setText("Changing the Depth dawg", dontSendNotification);
        }
        else if (slider->getComponentID().compare("Rate") == 0) {
            processor.flRateVal = slider->getValue();
        }
        else if (slider->getComponentID().compare("Feedback") == 0) {
            processor.flFeedbackVal = slider->getValue();
        }
        
        processor.updateFlanger();
    }
    // Update Phaser
    else if (modMenu.getSelectedItemIndex() == 1)
    {
        if (slider->getComponentID().compare("Mix") == 0) {
            processor.phsMixVal = slider->getValue();
        }
        else if (slider->getComponentID().compare("Depth") == 0) {
            processor.phsDepthVal = slider->getValue();
        }
        else if (slider->getComponentID().compare("Rate") == 0) {
            processor.phsRateVal = slider->getValue();
        }
        else if (slider->getComponentID().compare("Feedback") == 0) {
            processor.phsFeedbackVal = slider->getValue();
        }
        
        processor.updatePhaser();
    }
}

// Update our GUI after menu box change
// AUTOMATIC CALLBACK
void NebuloModAudioProcessorEditor::comboBoxChanged(ComboBox *comboBoxThatHasChanged)
{
    // Set our LFO
    if (lfoMenu.getSelectedItemIndex() == 0)
    {
        debugText.setText("Switching to the sine!!!", dontSendNotification);
        LFO.waveForm = 0;
        if (processor.flanger_active)
        {
            processor.flLfoWaveformVal = LFO.waveForm;
            processor.updateFlanger();
        }
        else if (processor.phaser_active)
        {
            processor.phsLfoWaveformVal = LFO.waveForm;
            processor.updatePhaser();
        }
        
        repaint(20, 160, 280, 250);
    }
    else if (lfoMenu.getSelectedItemIndex() == 1)
    {
        debugText.setText("Switching to the tri!!!", dontSendNotification);
        LFO.waveForm = 1;
        if (processor.flanger_active)
        {
            processor.flLfoWaveformVal = LFO.waveForm;
            processor.updateFlanger();
        }
        else if (processor.phaser_active)
        {
            processor.phsLfoWaveformVal = LFO.waveForm;
            processor.updatePhaser();
        }
        
        repaint(20, 160, 280, 250);
    }
    else if (lfoMenu.getSelectedItemIndex() == 2)
    {
        debugText.setText("Switching to the saw!!!", dontSendNotification);
        LFO.waveForm = 2;
        if (processor.flanger_active)
        {
            processor.flLfoWaveformVal = LFO.waveForm;
            processor.updateFlanger();
        }
        else if (processor.phaser_active)
        {
            processor.phsLfoWaveformVal = LFO.waveForm;
            processor.updatePhaser();
        }
        
        repaint(20, 160, 280, 250);
    }
    else if (lfoMenu.getSelectedItemIndex() == 3)
    {
        debugText.setText("Switching to the square!!!", dontSendNotification);
        LFO.waveForm = 3;
        if (processor.flanger_active)
        {
            processor.flLfoWaveformVal = LFO.waveForm;
            processor.updateFlanger();
        }
        else if (processor.phaser_active)
        {
            processor.phsLfoWaveformVal = LFO.waveForm;
            processor.updatePhaser();
        }
        
        repaint(20, 160, 280, 250);
    }
    else if (lfoMenu.getSelectedItemIndex() == 4)
    {
        debugText.setText("Switching to the custom!!!", dontSendNotification);
        LFO.waveForm = 4;
        if (processor.flanger_active)
        {
            processor.flLfoWaveformVal = LFO.waveForm;
            processor.updateFlanger();
        }
        else if (processor.phaser_active)
        {
            processor.phsLfoWaveformVal = LFO.waveForm;
            processor.updatePhaser();
        }
        
        repaint(20, 160, 280, 250);
    }
    
    // Switch to phaser...
    if (processor.flanger_active && (modMenu.getSelectedItemIndex() == 1))
    {
        debugText.setText("Switching to the phaser!!!", dontSendNotification);
        
        // Set flags
        processor.flanger_active = false;
        processor.phaser_active = true;
        
        // Disable our feedback slider since we are using a phaser
        feedbackSlider.setEnabled(false);
        
        // Change the necessary ranges
        rateSlider.setRange(1.0, 15.0, .01);
        
        // Text
        fxText.setText("Phaser", dontSendNotification);
        
        // Revert to saved phaser slider values
        depthSlider.setValue(processor.phsDepthVal);
        rateSlider.setValue(processor.phsRateVal);
        feedbackSlider.setValue(processor.phsFeedbackVal);
        mixSlider.setValue(processor.phsMixVal);
        lfoMenu.setSelectedItemIndex(processor.phsLfoWaveformVal);
        
        // Set our LFO
        if (lfoMenu.getSelectedItemIndex() == 0)
        {
            processor.phsLfoWaveformVal = 0;
        }
        else if (lfoMenu.getSelectedItemIndex() == 1)
        {
            processor.phsLfoWaveformVal = 1;
        }
        else if (lfoMenu.getSelectedItemIndex() == 2)
        {
            processor.phsLfoWaveformVal = 2;
        }
        else if (lfoMenu.getSelectedItemIndex() == 3)
        {
            processor.phsLfoWaveformVal = 3;
        }
        LFO.waveForm = processor.phsLfoWaveformVal;
        
        // Update the Phaser variables!
        processor.updatePhaser();
        
        // Repaint the GUI for new waveform drawing!
        repaint(20, 160, 280, 250);
    }
    // Switch to flanger...
    else if (processor.phaser_active && (modMenu.getSelectedItemIndex() == 0))
    {
        debugText.setText("Switching to the flanger!!!", dontSendNotification);
        
        // Set flags
        processor.flanger_active = true;
        processor.phaser_active = false;
        
        // Enable the feedback slider
        feedbackSlider.setEnabled(true);
        
        rateSlider.setRange(0.1, 5.0, .01);
        
        // Text
        fxText.setText("Flanger", dontSendNotification);
        
        // Revert to saved flanger slider values
        depthSlider.setValue(processor.flDepthVal);
        rateSlider.setValue(processor.flRateVal);
        feedbackSlider.setValue(processor.flFeedbackVal);
        mixSlider.setValue(processor.flMixVal);
        lfoMenu.setSelectedItemIndex(processor.flLfoWaveformVal);
        
        // Set our LFO
        if (lfoMenu.getSelectedItemIndex() == 0)
        {
            processor.flLfoWaveformVal = 0;
        }
        else if (lfoMenu.getSelectedItemIndex() == 1)
        {
            processor.flLfoWaveformVal = 1;
        }
        else if (lfoMenu.getSelectedItemIndex() == 2)
        {
            processor.flLfoWaveformVal = 2;
        }
        else if (lfoMenu.getSelectedItemIndex() == 3)
        {
            processor.flLfoWaveformVal = 3;
        }
        LFO.waveForm = processor.flLfoWaveformVal;
        
        // Update the Flanger variables!
        processor.updateFlanger();
        
        // Repaint the GUI for new waveform drawing!
        repaint(20, 160, 280, 250);
    }
    
    // Visualize our text!
    addAndMakeVisible(fxText);
    fxText.setVisible(true);
}

// Update GUI on mouse drag
void NebuloModAudioProcessorEditor::mouseDrag(const MouseEvent& event)
{
    if (LFO.waveForm == 4)
    {
        // printf("Getting location...X:%d Y:%d\n", event.x, event.y);
        if ((event.x >= 90) && (event.x <= 110) &&
            (event.y >= 170) && (event.y <= 190))
        {
            printf("Marker one found!\n");
            isMoving_marker_one = true;
            //repaint();
        }
        
        if ((event.x >= 210) && (event.x <= 230) &&
            (event.y >= 370) && (event.y <= 390))
        {
            printf("Marker two found!\n");
            isMoving_marker_two = true;
            //repaint();
        }
    }
}

void NebuloModAudioProcessorEditor::mouseEnter (const MouseEvent& event)
{
    // printf("Getting location...%d %d\n", event.x, event.y);
}

void NebuloModAudioProcessorEditor::mouseUp (const MouseEvent& event)
{
    printf("Released\n");
    
    if (isMoving_marker_one)
    {
        isMoving_marker_one = false;
        //our_customTable[255] = ;
    }
    
    else if (isMoving_marker_two)
    {
        isMoving_marker_two = false;
        //our_customTable[765] = ;
    }
    
}

void NebuloModAudioProcessorEditor::updatePath()
{
    // Clear the waveform
    waveRef.clear();
    
    waveRef.startNewSubPath(40, 280);
    // waveRef.lineTo(270, 280);
    for (int i = 0; i < 1024; i++)
    {
        // X-axis and Y-axis points
        float x_axis = (i+1) * 15 / 64;
        static float y_axis = 0;

        // Sine Wave Drawing
        if (LFO.waveForm == 0)
        {
            y_axis = our_sineTable[i] * -100;
        }
        // Triangle Wave Drawing
        else if (LFO.waveForm == 1)
        {
            if (i < 256)
                y_axis = (i+1) * -25 / 64;              // get to 180
            else if (i < 768)
                y_axis = ((i+1) * 25 / 64) - 200;       // get to 390
            else if (i < 1023)
                y_axis = ((i+1) * -25 / 64) + 400;      // get to 280
        }
        // Sawtooth Wave Drawing
        else if (LFO.waveForm == 2)
        {
            if (i == 0)
                y_axis = 110;
            else if (i < 1023)
                y_axis = (i+1) * -105/512 + 110;
            else
                y_axis = 55;
        }
        // Square Wave Drawing
        else if (LFO.waveForm == 3)
        {
            if (i < 512)
                y_axis = -100;
            else if (i < 1023)
                y_axis = 110;
            else
                y_axis = 0;
        }
        // Customized Wave Drawing
        else if (LFO.waveForm == 4)
        {
            y_axis = our_customTable[i] * -100;
            if (i == 240)
            {
                waveRef.addEllipse(40 + x_axis, 280 + y_axis - 5, 10, 10);
                marker_one.setXY(40 + x_axis, 280 + y_axis - 5);
            }
            else if (i == 750)
            {
                waveRef.addEllipse(40 + x_axis, 280 + y_axis - 5, 10, 10);
                marker_two.setXY(40 + x_axis, 280 + y_axis - 5);
            }
        }
        
        // Draw the Waveform!
        waveRef.lineTo(40 + x_axis, 280 + y_axis);
    }
}