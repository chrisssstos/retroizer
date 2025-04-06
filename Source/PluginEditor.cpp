/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
RetroizerAudioProcessorEditor::RetroizerAudioProcessorEditor(RetroizerAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    // Set up the bit depth slider
    bitDepthSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    bitDepthSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 90, 20);
    bitDepthSlider.setColour(juce::Slider::thumbColourId, juce::Colours::orangered);
    addAndMakeVisible(bitDepthSlider);

    bitDepthLabel.setText("Bit Depth", juce::dontSendNotification);
    bitDepthLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(bitDepthLabel);

    // Set up the sample rate slider
    sampleRateSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    sampleRateSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 90, 20);
    sampleRateSlider.setColour(juce::Slider::thumbColourId, juce::Colours::orangered);
    addAndMakeVisible(sampleRateSlider);

    sampleRateLabel.setText("Sample Rate", juce::dontSendNotification);
    sampleRateLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(sampleRateLabel);

    // Set up the radio mix 1 slider
    radioMix1Slider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    radioMix1Slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 90, 20);
    radioMix1Slider.setColour(juce::Slider::thumbColourId, juce::Colours::skyblue);
    addAndMakeVisible(radioMix1Slider);

    radioMix1Label.setText("Radio Mix 1", juce::dontSendNotification);
    radioMix1Label.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(radioMix1Label);

    // Set up the radio mix 2 slider
    radioMix2Slider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    radioMix2Slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 90, 20);
    radioMix2Slider.setColour(juce::Slider::thumbColourId, juce::Colours::skyblue);
    addAndMakeVisible(radioMix2Slider);

    radioMix2Label.setText("Radio Mix 2", juce::dontSendNotification);
    radioMix2Label.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(radioMix2Label);

    // Create parameter attachments
    bitDepthAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "bitDepth", bitDepthSlider);

    sampleRateAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "sampleRate", sampleRateSlider);

    radioMix1Attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "radioMix1", radioMix1Slider);

    radioMix2Attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "radioMix2", radioMix2Slider);

    // Set the plugin window size
    setSize(400, 300);
}

RetroizerAudioProcessorEditor::~RetroizerAudioProcessorEditor()
{
}

//==============================================================================
void RetroizerAudioProcessorEditor::paint(juce::Graphics& g)
{
    // Fill background with dark gray
    g.fillAll(juce::Colour(40, 42, 45));

    // Draw a gradient title bar
    juce::ColourGradient gradient(
        juce::Colour(60, 60, 65), 0, 0,
        juce::Colour(30, 30, 35), 0, 40,
        false);
    g.setGradientFill(gradient);
    g.fillRect(0, 0, getWidth(), 40);

    // Draw plugin title
    g.setColour(juce::Colours::white);
    g.setFont(24.0f);
    g.drawText("RETROIZER", getLocalBounds().removeFromTop(40), juce::Justification::centred);

    // Draw section lines
    g.setColour(juce::Colours::grey);
    g.drawLine(getWidth() / 2, 50, getWidth() / 2, getHeight() - 10, 1.0f);
    g.drawLine(10, 140, getWidth() - 10, 140, 1.0f);

    // Draw section titles
    g.setFont(16.0f);
    g.setColour(juce::Colours::orangered);
    g.drawText("BIT CRUSHER", 10, 45, getWidth() / 2 - 20, 20, juce::Justification::centred);
    g.setColour(juce::Colours::skyblue);
    g.drawText("RADIO EFFECT", getWidth() / 2 + 10, 45, getWidth() / 2 - 20, 20, juce::Justification::centred);
}

void RetroizerAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds();
    bounds.removeFromTop(70); // Space for title

    // Calculate areas for each control
    int halfWidth = getWidth() / 2;
    int controlHeight = (getHeight() - 70) / 2;

    // Top row
    auto topRow = bounds.removeFromTop(controlHeight);

    // Bit depth (top left)
    auto bitDepthArea = topRow.removeFromLeft(halfWidth);
    bitDepthSlider.setBounds(bitDepthArea.reduced(10).removeFromTop(80));
    bitDepthLabel.setBounds(bitDepthArea.reduced(10).removeFromTop(20));

    // Radio mix 1 (top right)
    radioMix1Slider.setBounds(topRow.reduced(10).removeFromTop(80));
    radioMix1Label.setBounds(topRow.reduced(10).removeFromTop(20));

    // Bottom row
    // Sample rate (bottom left)
    auto sampleRateArea = bounds.removeFromLeft(halfWidth);
    sampleRateSlider.setBounds(sampleRateArea.reduced(10).removeFromTop(80));
    sampleRateLabel.setBounds(sampleRateArea.reduced(10).removeFromTop(20));

    // Radio mix 2 (bottom right)
    radioMix2Slider.setBounds(bounds.reduced(10).removeFromTop(80));
    radioMix2Label.setBounds(bounds.reduced(10).removeFromTop(20));
}