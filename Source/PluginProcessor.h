//------------------------------------------------------------------------------
//  Code for the plugin's audio processing logic
//------------------------------------------------------------------------------

#pragma once

#include "CircularBuffer.h"

#include <JuceHeader.h>

#define DEBUG_MODE 0

#if DEBUG_MODE
#include "ToneGenerator.h"
#endif

//------------------------------------------------------------------------------
class PlaybackRateModulatorAudioProcessor : public juce::AudioProcessor
{
public:
	PlaybackRateModulatorAudioProcessor();
	~PlaybackRateModulatorAudioProcessor() override {};

	bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
	void prepareToPlay(double sampleRate, int samplesPerBlock) override;
	void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
	void releaseResources() override {};

	void getStateInformation(juce::MemoryBlock& destData) override;
	void setStateInformation(const void* data, int sizeInBytes) override;

	juce::AudioProcessorEditor* createEditor() override;

	//--------------------------------------
	// Mandatory, one-liner overrides
	//--------------------------------------
	bool hasEditor() const override { return false; }
	bool acceptsMidi() const override { return false; }
	bool producesMidi() const override { return false; }
	bool isMidiEffect() const override { return false; }
	double getTailLengthSeconds() const override { return 0.0; }
	int getNumPrograms() override { return 1; }
	int getCurrentProgram() override { return 0; }
	const juce::String getName() const override { return JucePlugin_Name; }
	const juce::String getProgramName(int index) override { return {}; }
	void changeProgramName(int index, const juce::String& newName) override {}
	void setCurrentProgram(int index) override {}

private:
	CircularBuffer m_buffer[2];
	std::atomic<float>* m_pRateParam;
	std::atomic<float>* m_pBufferSizeParam;;
	float m_delayLastFrame; // delay (samples) of the last sample of the last frame
	float m_bufferLengthLast;

#if DEBUG_MODE
	ToneGenerator m_toneGenerator;
#endif

	juce::AudioProcessorValueTreeState m_params;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlaybackRateModulatorAudioProcessor)
};
