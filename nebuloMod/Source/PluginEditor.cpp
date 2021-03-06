/*
  ==============================================================================

    This file was auto-generated by the Introjucer!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

// Written by Ryan Foo

#include "PluginEditor.h"

// Thanks Uri!!! So helpful man!
// Creates a Slider with customized params
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

// Creates a label with customized params
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
    createSlider(feedbackSlider, Slider::LinearHorizontal, processor.flFeedbackVal, 0.0, 1.0, 0.01, "Feedback");
    createSlider(mixSlider, Slider::LinearHorizontal, processor.flMixVal, 0.0, 0.5, 0.01, "Mix");
    
    // Create texts!
    createLabel(depthText, "Depth");
    createLabel(rateText, "Rate");
    createLabel(feedbackText, "Feedback");
    createLabel(mixText, "Mix");
    
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
    
    // Inits: (flanger enabled)
    modMenu.setSelectedItemIndex(0);
    lfoMenu.setSelectedItemIndex(0);
    LFO.waveForm = 0;
    feedbackSlider.setEnabled(true);
    fxText.setText("Flanger", dontSendNotification);
    
    // Update processor
    processor.flLfoWaveformVal = LFO.waveForm;
    processor.phsLfoWaveformVal = LFO.waveForm;
    
    // Update Phaser
    processor.updateFlanger();
    
    // Waveform Component
    addAndMakeVisible(&wavComponent);
    
    // LFO Reset Button
    resetLFO.setEnabled(false);
    resetLFO.setSize(100, 25);
    resetLFO.setButtonText("Reset LFO");
    resetLFO.addListener(this);
    addAndMakeVisible(resetLFO);
    
    // Flags
    processor.flanger_active = true;
    processor.phaser_active = false;
}

NebuloModAudioProcessorEditor::~NebuloModAudioProcessorEditor()
{
}

//==============================================================================
void NebuloModAudioProcessorEditor::paint (Graphics& g)
{
    // Background
    g.fillAll (Colours::black);
    
    // Texts and Line Graph
    g.setColour (Colours::white);
    g.setFont (60.0f);
    g.drawFittedText ("Nebulo Mod", getLocalBounds(), Justification::bottomLeft, 1);
    g.setFont(16.0f);
    g.drawFittedText("by Ryan Foo and Tom Longabaugh", getLocalBounds(), Justification::bottomRight, 2);
    
    // Backgrounds son!!!
    g.setColour(Colours::grey);
    g.fillRoundedRectangle(320, 160, 370, 250, 25);
    g.fillRoundedRectangle(20, 160, 280, 250, 25);
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
    
    // Buttons!
    resetLFO.setBounds(110, 430, 75, 25);
    resetLFO.changeWidthToFitText();
    
    // Waveform Component
    wavComponent.setBounds(35, 180, 250, 210);          // 250 210
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
    /*
     * Set our LFO when the LFO Box is changed
     * Process of how LFO is changed and set:
     * 1) Check which item is selected (sine wave, tri wave, etc)
     * 2) Set LFOWaveformTable waveForm
     * 3) Check active effect flag
     * 4) Set Processor's Waveform Value (for saving) and update effect
     * 5) Enable GUI point markers if we are using Custom selection. O/W set to false
     * 6) Enable Reset LFO button if we are using Custom selection. O/W set to false
     * 7) Refresh the line path graph to correct drawing
     */
    // Sine Wave
    if (lfoMenu.getSelectedItemIndex() == 0)
    {
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
        
        wavComponent.enablePoints(false);
        resetLFO.setEnabled(false);
        wavComponent.refreshPath(LFO.waveForm);
    }
    // Triangle Wave
    else if (lfoMenu.getSelectedItemIndex() == 1)
    {
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
        
        wavComponent.enablePoints(false);
        resetLFO.setEnabled(false);
        wavComponent.refreshPath(LFO.waveForm);
    }
    // Sawtooth Wave
    else if (lfoMenu.getSelectedItemIndex() == 2)
    {
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

        wavComponent.enablePoints(false);
        resetLFO.setEnabled(false);
        wavComponent.refreshPath(LFO.waveForm);
    }
    // Square Wave
    else if (lfoMenu.getSelectedItemIndex() == 3)
    {
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
        
        wavComponent.enablePoints(false);
        resetLFO.setEnabled(false);
        wavComponent.refreshPath(LFO.waveForm);
    }
    // Custom Wave
    else if (lfoMenu.getSelectedItemIndex() == 4)
    {
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
        
        wavComponent.enablePoints(true);
        resetLFO.setEnabled(true);
        wavComponent.refreshPath(LFO.waveForm);
    }
    
    /**
     * Set our Modulation Effect when the Effect Box is Changed
     * Process of how Modulation Effect is changed:
     * 1) Check flag status and which item is selected
     * 2) Set new flags
     * 3) Enable feedback slider depending on the selected effect
     * 4) Reset slider ranges
     * 5) Revert to saved slider values of effect
     * 6) Set LFO
     * 7) Update Effect Status
     * 8) Refresh line path graph
     */
    
    // Switch to phaser...
    if (processor.flanger_active && (modMenu.getSelectedItemIndex() == 1))
    {
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
            resetLFO.setEnabled(false);
        }
        else if (lfoMenu.getSelectedItemIndex() == 1)
        {
            processor.phsLfoWaveformVal = 1;
            resetLFO.setEnabled(false);
        }
        else if (lfoMenu.getSelectedItemIndex() == 2)
        {
            processor.phsLfoWaveformVal = 2;
            resetLFO.setEnabled(false);
        }
        else if (lfoMenu.getSelectedItemIndex() == 3)
        {
            processor.phsLfoWaveformVal = 3;
            resetLFO.setEnabled(false);
        }
        else if (lfoMenu.getSelectedItemIndex() == 4)
        {
            processor.phsLfoWaveformVal = 4;
            resetLFO.setEnabled(true);
        }
        LFO.waveForm = processor.phsLfoWaveformVal;
        
        // Update the Phaser variables!
        processor.updatePhaser();
        
        // Repaint the GUI for new waveform drawing!
        wavComponent.refreshPath(LFO.waveForm);
    }
    // Switch to flanger...
    else if (processor.phaser_active && (modMenu.getSelectedItemIndex() == 0))
    {
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
            resetLFO.setEnabled(false);
        }
        else if (lfoMenu.getSelectedItemIndex() == 1)
        {
            processor.flLfoWaveformVal = 1;
            resetLFO.setEnabled(false);
        }
        else if (lfoMenu.getSelectedItemIndex() == 2)
        {
            processor.flLfoWaveformVal = 2;
            resetLFO.setEnabled(false);
        }
        else if (lfoMenu.getSelectedItemIndex() == 3)
        {
            processor.flLfoWaveformVal = 3;
            resetLFO.setEnabled(false);
        }
        else if (lfoMenu.getSelectedItemIndex() == 4)
        {
            processor.flLfoWaveformVal = 4;
            resetLFO.setEnabled(true);
        }
        LFO.waveForm = processor.flLfoWaveformVal;
        
        // Update the Flanger variables!
        processor.updateFlanger();
        
        // Repaint the GUI for new waveform drawing!
        wavComponent.refreshPath(LFO.waveForm);
    }
    
    // Visualize our text!
    addAndMakeVisible(fxText);
    fxText.setVisible(true);
}

// Button Click Override
// When the button is clicked, reset the buffer
void NebuloModAudioProcessorEditor::buttonClicked(Button* b)
{
    // If it's our button, reset the buffer
    if (b)
    {
        wavComponent.resetBuffer();
    }
}

// Button State Change Override
// Don't do anything here
void NebuloModAudioProcessorEditor::buttonStateChanged(Button* b)
{

}