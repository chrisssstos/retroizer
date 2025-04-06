#pragma once
#include <JuceHeader.h>

class RadioEffect
{
public:
    RadioEffect()
    {
        updateFilter1(800.0f, 0.5f);
        updateFilter2(1200.0f, 0.7f);
    }
    
    void prepare(const juce::dsp::ProcessSpec& spec)
    {
        radioFilter1.prepare(spec);
        radioFilter2.prepare(spec);
        radioFilter1.reset();
        radioFilter2.reset();
    }
    
    void process(float* buffer, int numSamples)
    {
        if (mix1 == 0.0f && mix2 == 0.0f) return;
        
        juce::AudioBuffer<float> tempBuffer(1, numSamples);
        tempBuffer.copyFrom(0, 0, buffer, numSamples);
        
        if (mix1 > 0.0f)
        {
            juce::dsp::AudioBlock<float> block(tempBuffer);
            juce::dsp::ProcessContextReplacing<float> context(block);
            radioFilter1.process(context);
            
            for (int i = 0; i < numSamples; ++i)
                buffer[i] = buffer[i] * (1.0f - mix1) + tempBuffer.getSample(0, i) * mix1;
        }
        
        if (mix2 > 0.0f)
        {
            juce::dsp::AudioBlock<float> block(tempBuffer);
            juce::dsp::ProcessContextReplacing<float> context(block);
            radioFilter2.process(context);
            
            for (int i = 0; i < numSamples; ++i)
                buffer[i] = buffer[i] * (1.0f - mix2) + tempBuffer.getSample(0, i) * mix2;
        }
    }
    
    void updateFilter1(float freq, float q)
    {
        *radioFilter1.state = juce::dsp::IIR::ArrayCoefficients<float>::makeBandPass(
            sampleRate, freq, q);
    }
    
    void updateFilter2(float freq, float q)
    {
        *radioFilter2.state = juce::dsp::IIR::ArrayCoefficients<float>::makeBandPass(
            sampleRate, freq, q);
    }
    
    void setMix1(float newMix) { mix1 = newMix; }
    void setMix2(float newMix) { mix2 = newMix; }
    
private:
    juce::dsp::ProcessorDuplicator<
        juce::dsp::IIR::Filter<float>,
        juce::dsp::IIR::Coefficients<float>> radioFilter1, radioFilter2;
    
    float mix1 = 0.0f;
    float mix2 = 0.0f;
    double sampleRate = 44100.0;
};