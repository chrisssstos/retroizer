#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
RetroizerAudioProcessor::RetroizerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
    ),
#endif
    apvts(*this, nullptr, "Parameters", createParameterLayout())
{
}

RetroizerAudioProcessor::~RetroizerAudioProcessor()
{
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
    return 1;   // Some hosts don't cope well if you tell them there are 0 programs
}

int RetroizerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void RetroizerAudioProcessor::setCurrentProgram(int index)
{
    juce::ignoreUnused(index);
}

const juce::String RetroizerAudioProcessor::getProgramName(int index)
{
    juce::ignoreUnused(index);
    return {};
}

void RetroizerAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
    juce::ignoreUnused(index, newName);
}

//==============================================================================
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
    // When playback stops, you can use this to free up any spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool RetroizerAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    // Support stereo in/stereo out only
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    return true;
}
#endif

void RetroizerAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    // Avoid unused parameter warning
    juce::ignoreUnused(midiMessages);

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

//==============================================================================
bool RetroizerAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* RetroizerAudioProcessor::createEditor()
{
    return new RetroizerAudioProcessorEditor(*this);
}

//==============================================================================
void RetroizerAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void RetroizerAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr)
        apvts.replaceState(juce::ValueTree::fromXml(*xmlState));
}

//==============================================================================
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
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new RetroizerAudioProcessor();
}