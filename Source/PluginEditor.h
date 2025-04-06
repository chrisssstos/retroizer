#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class RetroizerAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    explicit RetroizerAudioProcessorEditor(RetroizerAudioProcessor&);
    ~RetroizerAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    RetroizerAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RetroizerAudioProcessorEditor)
};