/*
  ==============================================================================

    This file was auto-generated by the Introjucer!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
NebuloModAudioProcessor::NebuloModAudioProcessor()
{
}

NebuloModAudioProcessor::~NebuloModAudioProcessor()
{
}

//==============================================================================
const String NebuloModAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

int NebuloModAudioProcessor::getNumParameters()
{
    return 1;
}

float NebuloModAudioProcessor::getParameter (int index)
{
    return index;
}

void NebuloModAudioProcessor::setParameter (int index, float newValue)
{
}

const String NebuloModAudioProcessor::getParameterName (int index)
{
    return "";
}

const String NebuloModAudioProcessor::getParameterText (int index)
{
    return String(index);
}

const String NebuloModAudioProcessor::getInputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

const String NebuloModAudioProcessor::getOutputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

bool NebuloModAudioProcessor::isInputChannelStereoPair (int index) const
{
    return true;
}

bool NebuloModAudioProcessor::isOutputChannelStereoPair (int index) const
{
    return true;
}

bool NebuloModAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool NebuloModAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool NebuloModAudioProcessor::silenceInProducesSilenceOut() const
{
    return false;
}

double NebuloModAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int NebuloModAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int NebuloModAudioProcessor::getCurrentProgram()
{
    return 0;
}

void NebuloModAudioProcessor::setCurrentProgram (int index)
{
}

const String NebuloModAudioProcessor::getProgramName (int index)
{
    return String();
}

void NebuloModAudioProcessor::changeProgramName (int index, const String& newName)
{
}

// GUI elements implemented by Ryan Foo
void NebuloModAudioProcessor::updateFlanger(void)
{
    // set the params
    Flanger::Parameters flangerParams = flanger.getParameters();
    flangerParams.depth = flDepthVal;
    flangerParams.rate = flRateVal;
    flangerParams.lfoWaveform = flLfoWaveformVal;
    flangerParams.feedback = flFeedbackVal;
    flangerParams.mix = flMixVal;
    
    flanger.setParameters(flangerParams);
}

void NebuloModAudioProcessor::updatePhaser(void)
{
    // set the params
    Phaser::Parameters phaserParams = phaser.getParameters();
    phaserParams.depth = phsDepthVal;
    phaserParams.rate = phsRateVal;
    phaserParams.lfoWaveform = phsLfoWaveformVal;
    phaserParams.mix = phsMixVal;
    
    phaser.setParameters(phaserParams);
}

//==============================================================================
void NebuloModAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    Phaser::Parameters phaserParams = phaser.getParameters();
    Flanger::Parameters flangerParams = flanger.getParameters();
    
    // Get Flanger/Phaser Values:
    
    // set Volume
    flMixVal = flangerParams.mix;
    flDepthVal= flangerParams.depth;
    flRateVal = flangerParams.rate;
    flFeedbackVal = flangerParams.feedback;
    flLfoWaveformVal = flangerParams.lfoWaveform;
    
    // set Volume
    phsMixVal = phaserParams.mix;
    phsDepthVal= phaserParams.depth;
    phsRateVal = phaserParams.rate;
    phsFeedbackVal = 50.0;// never used
    phsLfoWaveformVal = phaserParams.lfoWaveform;
    
    // Set Our Flanger and phaser Sample Rate
    flanger.setSampleRate(getSampleRate());
    phaser.setSampleRate(getSampleRate());
    phaser.prepareToPlay();
    flanger.prepareToPlay();
}

void NebuloModAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void NebuloModAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // I've added this to avoid people getting screaming feedback
    // when they first compile the plugin, but obviously you don't need to
    // this code if your algorithm already fills all the output channels.
    for (int i = getNumInputChannels(); i < getNumOutputChannels(); ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    //Written by tom longabaugh and ryan foo
    
    /* Perform the necessary audio processing */
    
    
    /* Get the samples from the input buffer, then process them and send to output */
    // Mono -> Mono
    if (getNumInputChannels() == 1) {
        // Obtain data from channel 1
        float *monoData = buffer.getWritePointer(0);
        
        // Check Flag status to determine which effect to process
        if (phaser_active)
        {
            phaser.processMono(monoData, buffer.getNumSamples());
        }
        else if (flanger_active)
        {
            flanger.processMono(monoData, buffer.getNumSamples());
        }
        else
        {
            // do nothing
        }
    }// Mono -> Stereo
    /*
    if (getNumInputChannels() == 1 ) {
        // Obtain data from channel 1
        float *monoDataL = buffer.getWritePointer(0);
        float *monoDataR = buffer.getWritePointer(1);
        
        // Check Flag status to determine which effect to process
        if (phaser_active)
        {
            phaser.processMono(monoDataL, buffer.getNumSamples());
        }
        else if (flanger_active)
        {
            flanger.processMono(monoDataR, buffer.getNumSamples());
        }
        else
        {
            // do nothing
        }
    }*/
    else if (getNumInputChannels() == 2) {
        // Obtain data from channels 1 + 2
        float *leftChannel = buffer.getWritePointer(0);
        float *rightChannel = buffer.getWritePointer(1);

        // Check Flag status to determine which effect to process        
        if (phaser_active)
        {
            phaser.processStereo(leftChannel, rightChannel, buffer.getNumSamples());
        }
        else if (flanger_active)
        {
            flanger.processStereo(leftChannel, rightChannel, buffer.getNumSamples());
        }
        else
        {
            // do nothing son!
        }
    }
    else {
        // We should never get here, since plug-in can't initialize except for those in/out combos
    }
}


//==============================================================================
bool NebuloModAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* NebuloModAudioProcessor::createEditor()
{
    return new NebuloModAudioProcessorEditor (*this);
}

//==============================================================================
void NebuloModAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void NebuloModAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new NebuloModAudioProcessor();
}
