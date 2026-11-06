#include "PluginProcessor.h"
#include "PluginEditor.h"

ProbeProcessor::ProbeProcessor()
    : AudioProcessor (BusesProperties()
                          .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                          .withOutput ("Output", juce::AudioChannelSet::stereo(), true))
{
}

juce::AudioProcessorEditor* ProbeProcessor::createEditor()
{
    return new ProbeEditor (*this);
}

void ProbeProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    juce::MemoryOutputStream stream (destData, true);
    stream.writeString (dllPath);
}

void ProbeProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    juce::MemoryInputStream stream (data, static_cast<size_t> (sizeInBytes), false);
    dllPath = stream.readString();
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ProbeProcessor();
}
