#pragma once
#include <JuceHeader.h>

class BitCrusher
{
public:
    BitCrusher() = default;
    
    void prepare(const juce::dsp::ProcessSpec& spec)
    {
        sampleRate = spec.sampleRate;
        reset();
    }
    
    void process(float* buffer, int numSamples)
    {
        for (int i = 0; i < numSamples; ++i)
        {
            // Apply bit reduction
            float step = powf(0.5f, bitDepth);
            buffer[i] = floor(buffer[i] / step + 0.5f) * step;
            
            // Apply sample rate reduction
            if (sampleRateDivisor > 1)
            {
                if (sampleCount % sampleRateDivisor != 0)
                {
                    buffer[i] = holdSample;
                }
                else
                {
                    holdSample = buffer[i];
                }
                sampleCount++;
            }
        }
    }
    
    void setBitDepth(float depth) { bitDepth = juce::jmax(1.0f, depth * 15.0f + 1.0f); } // 1-16 bits
    void setSampleRateReduction(float amount) { sampleRateDivisor = juce::jmax(1, (int)(amount * 32.0f)); }
    void reset()
    {
        holdSample = 0.0f;
        sampleCount = 0;
    }
    
private:
    float bitDepth = 16.0f;
    int sampleRateDivisor = 1;
    float holdSample = 0.0f;
    int sampleCount = 0;
    double sampleRate = 44100.0;
};