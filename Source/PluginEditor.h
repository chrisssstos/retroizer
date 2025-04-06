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

    // Parameter sliders
    juce::Slider bitDepthSlider;
    juce::Slider sampleRateSlider;
    juce::Slider radioMix1Slider;
    juce::Slider radioMix2Slider;

    // Labels for sliders
    juce::Label bitDepthLabel;
    juce::Label sampleRateLabel;
    juce::Label radioMix1Label;
    juce::Label radioMix2Label;

    // Attachments for parameters
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> bitDepthAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sampleRateAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> radioMix1Attachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> radioMix2Attachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RetroizerAudioProcessorEditor)
};
