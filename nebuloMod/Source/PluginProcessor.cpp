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
    //gain = 1.0f;
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
    //return gain;
    return index;
}

void NebuloModAudioProcessor::setParameter (int index, float newValue)
{
    //gain = newValue;

}

const String NebuloModAudioProcessor::getParameterName (int index)
{
    return "PUT something HERE LATER";
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
    Flanger::Parameters flangerParams = flanger.getParameters();
    flangerParams.depth = flDepthVal;
    flangerParams.rate = flRateVal;
    flangerParams.lfoWaveform = flLfoWaveformVal;
    flangerParams.feedback = flFeedbackVal;
    flangerParams.mix = flMixVal;
}

void NebuloModAudioProcessor::updatePhaser(void)
{
    Phaser::Parameters phaserParams = phaser.getParameters();
    phaserParams.depth = phsDepthVal;
    phaserParams.rate = phsRateVal;
    phaserParams.lfoWaveform = phsLfoWaveformVal;
    phaserParams.mix = phsMixVal;
}

//==============================================================================
void NebuloModAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    
    // set Volume
    flMixVal = 0.5;
    flDepthVal= 0.5;
    flRateVal = 0.5;
    flFeedbackVal = 50.0;
    flLfoWaveformVal = 0;
    
    // set Volume
    phsMixVal = 0.5;
    phsDepthVal= 0.5;
    phsRateVal = 0.5;
    phsFeedbackVal = 50.0;
    phsLfoWaveformVal = 0;
    
    // Set Our Flanger and phaser Sample Rate
    flanger.setSampleRate(getSampleRate());
    phaser.setSampleRate(getSampleRate());
    phaser.prepareToPlay();
    
    //should call reset here
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
    
    //int sampleCount = buffer.getNumSamples();
    
    /* Get the samples from the input buffer */
    if (getNumInputChannels() == 1) {
        float *monoData = buffer.getWritePointer(0);
        phaser.processMono(monoData, buffer.getNumSamples());
    }
    else if (getNumInputChannels() == 2) {
        float *leftChannel = buffer.getWritePointer(0);
        float *rightChannel = buffer.getWritePointer(1);
        phaser.processStereo(leftChannel, rightChannel, buffer.getNumSamples());
    }
    else {
        // This is wrong, dont' do anything??
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
