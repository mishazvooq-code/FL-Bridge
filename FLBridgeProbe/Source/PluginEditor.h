#pragma once

#include "PluginProcessor.h"
#include <juce_gui_basics/juce_gui_basics.h>

class ProbeEditor final : public juce::AudioProcessorEditor
{
public:
    explicit ProbeEditor (ProbeProcessor&);

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    void browse();
    void runProbe();

    ProbeProcessor& proc;

    juce::TextEditor pathBox, logBox;
    juce::TextButton browseButton { "Browse..." }, probeButton { "Probe" };
    std::unique_ptr<juce::FileChooser> chooser;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ProbeEditor)
};
