/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
RetroizerAudioProcessorEditor::RetroizerAudioProcessorEditor (RetroizerAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
}

RetroizerAudioProcessorEditor::~RetroizerAudioProcessorEditor()
{
}

//==============================================================================
void RetroizerAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (15.0f));
    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void RetroizerAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
#include "PluginProcessor.h"
#include "PluginEditor.h"

RetroizerAudioProcessor::RetroizerAudioProcessor()
    : apvts(*this, nullptr, "Parameters", createParameterLayout())
{
}

RetroizerAudioProcessor::~RetroizerAudioProcessor()
{
}

void RetroizerAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();
    
    bitCrusher.prepare(spec);
    radioEffect.prepare(spec);
}

void RetroizerAudioProcessor::releaseResources()
{
}

void RetroizerAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    
    // Get parameter values
    auto bitDepthParam = apvts.getRawParameterValue("bitDepth");
    auto sampleRateParam = apvts.getRawParameterValue("sampleRate");
    auto radioMix1Param = apvts.getRawParameterValue("radioMix1");
    auto radioMix2Param = apvts.getRawParameterValue("radioMix2");
    
    // Update DSP parameters
    bitCrusher.setBitDepth(bitDepthParam->load());
    bitCrusher.setSampleRateReduction(sampleRateParam->load());
    radioEffect.setMix1(radioMix1Param->load());
    radioEffect.setMix2(radioMix2Param->load());
    
    // Process each channel
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel);
        
        // Apply effects
        bitCrusher.process(channelData, buffer.getNumSamples());
        radioEffect.process(channelData, buffer.getNumSamples());
    }
}

juce::AudioProcessorValueTreeState::ParameterLayout RetroizerAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    
    // Bit Crusher parameters
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "bitDepth", "Bit Depth",
        juce::NormalisableRange<float>(0.0f, 1.0f), 0.0f));
    
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "sampleRate", "Sample Rate Reduction",
        juce::NormalisableRange<float>(0.0f, 1.0f), 0.0f));
    
    // Radio Effect parameters
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "radioMix1", "Radio Mix 1",
        juce::NormalisableRange<float>(0.0f, 1.0f), 0.0f));
    
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "radioMix2", "Radio Mix 2",
        juce::NormalisableRange<float>(0.0f, 1.0f), 0.0f));
    
    return layout;
}

//==============================================================================
const juce::String RetroizerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool RetroizerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool RetroizerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool RetroizerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double RetroizerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int RetroizerAudioProcessor::getNumPrograms()
{
    return 1;   // Some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int RetroizerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void RetroizerAudioProcessor::setCurrentProgram (int index)
{
    juce::ignoreUnused(index);
}

const juce::String RetroizerAudioProcessor::getProgramName (int index)
{
    juce::ignoreUnused(index);
    return {};
}

void RetroizerAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
    juce::ignoreUnused(index, newName);
}

//==============================================================================
bool RetroizerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* RetroizerAudioProcessor::createEditor()
{
    return new RetroizerAudioProcessorEditor (*this);
}

//==============================================================================
void RetroizerAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // Save parameters
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void RetroizerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // Load parameters
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState.get() != nullptr)
        apvts.replaceState(juce::ValueTree::fromXml(*xmlState));
}

//==============================================================================
// This creates new instances of the plugin
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new RetroizerAudioProcessor();
}