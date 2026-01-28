#include "PluginProcessor.h"
#include "PluginEditor.h"

//------------------------------------------------------------------------------
PlaybackRateModulatorAudioProcessor::PlaybackRateModulatorAudioProcessor() :
	AudioProcessor
	(
		BusesProperties()
			.withInput("Input", juce::AudioChannelSet::stereo(), true)
			.withOutput("Output", juce::AudioChannelSet::stereo(), true)
	)
{
}

//------------------------------------------------------------------------------
bool PlaybackRateModulatorAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
	// Support mono or stereo output
	if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
		&& layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
		return false;

	// Check if the input layout matches the output layout
	if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
		return false;

	return true;
}

//------------------------------------------------------------------------------
void PlaybackRateModulatorAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
}

//------------------------------------------------------------------------------
void PlaybackRateModulatorAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
	juce::ScopedNoDenormals noDenormals;

	const int nOutputChannels = std::min(getTotalNumOutputChannels(), 2);

	for (int iChan = 0; iChan < nOutputChannels; ++iChan)
	{
		float* pDst = buffer.getWritePointer(iChan);

		// ..do something to the data...
	}
}

//------------------------------------------------------------------------------
void PlaybackRateModulatorAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
}

//------------------------------------------------------------------------------
void PlaybackRateModulatorAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
}

//------------------------------------------------------------------------------
juce::AudioProcessorEditor* PlaybackRateModulatorAudioProcessor::createEditor()
{
	return new PlaybackRateModulatorAudioProcessorEditor(*this);
}

//------------------------------------------------------------------------------
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
	return new PlaybackRateModulatorAudioProcessor();
}