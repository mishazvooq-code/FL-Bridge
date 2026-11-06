#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

// Pass-through effect. It exists only to host the editor that probes an FL plugin DLL.
class ProbeProcessor final : public juce::AudioProcessor
{
public:
    ProbeProcessor();

    const juce::String getName() const override { return "FL Bridge Probe"; }

    void prepareToPlay (double, int) override {}
    void releaseResources() override {}

    bool isBusesLayoutSupported (const BusesLayout& layouts) const override
    {
        const auto& out = layouts.getMainOutputChannelSet();
        return ! out.isDisabled() && out == layouts.getMainInputChannelSet();
    }

    using AudioProcessor::processBlock;
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override {} // audio passes through untouched

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    bool acceptsMidi() const override  { return false; }
    bool producesMidi() const override { return false; }
    double getTailLengthSeconds() const override { return 0.0; }

    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram (int) override {}
    const juce::String getProgramName (int) override { return {}; }
    void changeProgramName (int, const juce::String&) override {}

    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    juce::String dllPath; // saved with the project

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ProbeProcessor)
};
