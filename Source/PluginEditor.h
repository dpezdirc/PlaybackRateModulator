//------------------------------------------------------------------------------
//  Code for the plugin's GUI
//------------------------------------------------------------------------------

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//------------------------------------------------------------------------------
class PlaybackRateModulatorAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
	PlaybackRateModulatorAudioProcessorEditor(PlaybackRateModulatorAudioProcessor&);
	~PlaybackRateModulatorAudioProcessorEditor() override {}

	void paint(juce::Graphics&) override;
	void resized() override;

private:
	PlaybackRateModulatorAudioProcessor& audioProcessor;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlaybackRateModulatorAudioProcessorEditor)
};
