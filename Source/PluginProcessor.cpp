#include "PluginProcessor.h"
#include "PluginEditor.h"

//------------------------------------------------------------------------------
PlaybackRateModulatorAudioProcessor::PlaybackRateModulatorAudioProcessor() :
	AudioProcessor
	(
		BusesProperties()
			.withInput("Input", juce::AudioChannelSet::stereo(), true)
			.withOutput("Output", juce::AudioChannelSet::stereo(), true)
	),
	m_bufferLengthLast(5.f),
	m_delayLastFrame(0.f),
	m_params(*this, nullptr, juce::Identifier("PlaybackRateModulatorParams"),
		{
			std::make_unique<juce::AudioParameterFloat>("bufferLength", "Buffer Length", juce::NormalisableRange<float>(0.1f, 10.f, 0.1f), m_bufferLengthLast),
			std::make_unique<juce::AudioParameterFloat>("rate", "Rate", juce::NormalisableRange<float>(-5.f, 5.f, 0.001f), 1.f),
		})
{
	m_pRateParam = m_params.getRawParameterValue("rate");
	m_pBufferSizeParam = m_params.getRawParameterValue("bufferLength");
}

//------------------------------------------------------------------------------
void PlaybackRateModulatorAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
	m_buffer[0].Resize(static_cast<int>(std::ceil(m_pBufferSizeParam->load() * sampleRate)));
	m_buffer[1].Resize(static_cast<int>(std::ceil(m_pBufferSizeParam->load() * sampleRate)));

#if DEBUG_MODE
	m_toneGenerator = ToneGenerator(sampleRate, 440, -12.f);
#endif
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
void PlaybackRateModulatorAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
	juce::ScopedNoDenormals noDenormals;

	const int nInputChannels = getTotalNumInputChannels();
	const int nOutputChannels = getTotalNumOutputChannels();
	const int nSamples = buffer.getNumSamples();
	const auto sampleRate = getSampleRate();
	
	for (auto i = nInputChannels; i < nOutputChannels; ++i)
		buffer.clear(i, 0, buffer.getNumSamples());

#if DEBUG_MODE
	m_toneGenerator.FillBuffer(buffer.getArrayOfWritePointers(), nSamples, nOutputChannels);
#endif

	const float rate = m_pRateParam->load();

	// calculate the delay of the last sample of this frame
	const float lastSampleDelay = m_delayLastFrame + (1.f - rate) * nSamples;

	// check if we have enough space in the buffer to accommodate the delay
	if (static_cast<int>(ceil(lastSampleDelay)) >= m_buffer[0].GetSize() - nSamples)
		return;

	const float bufferLength = m_pBufferSizeParam->load();

	// resize buffer if requested
	if (bufferLength != m_bufferLengthLast)
	{
		const int bufferSizeSamples = std::ceil(bufferLength * sampleRate);
		m_buffer[0].Resize(bufferSizeSamples);
		m_buffer[1].Resize(bufferSizeSamples);
		m_bufferLengthLast = bufferLength;
	}

	for (int iChan = 0; iChan < nInputChannels; ++iChan)
	{
		auto* pData = buffer.getWritePointer(iChan);

		// We write to the current buffer before processing for simplicity -
		// this way the delay buffer contains all the samples that we need and we don't have to worry about the current buffer.
		m_buffer[iChan].WriteSamples(pData, nSamples);

		for (int n = 0; n < nSamples; n++)
		{
			const float dDelay = lastSampleDelay - m_delayLastFrame; // change in delay between previous and current frame
			const float delay = std::max(m_delayLastFrame + dDelay * static_cast<float>(n) / nSamples, 0.f); // delay of current sample
			const int idxPre = static_cast<int>(std::ceil(delay));
			const int idxPost = static_cast<int>(std::floor(delay));
			const float samplePre = m_buffer[iChan][n - nSamples + 1 - idxPre];
			const float samplePost = m_buffer[iChan][n - nSamples + 1 - idxPost];
			const float weight = idxPre - delay; // 0 at samplePre, 1 at samplePost
			const float sampleInterp = samplePre + (samplePost - samplePre) * weight;

			pData[n] = sampleInterp;
		}
	}

	m_delayLastFrame = std::max(lastSampleDelay, 0.f);
}

//------------------------------------------------------------------------------
juce::AudioProcessorEditor* PlaybackRateModulatorAudioProcessor::createEditor()
{
	return new PlaybackRateModulatorAudioProcessorEditor(*this);
}

//------------------------------------------------------------------------------
void PlaybackRateModulatorAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
	juce::ValueTree state = m_params.copyState();
	std::unique_ptr<juce::XmlElement> xml(state.createXml());
	copyXmlToBinary(*xml, destData);
}

//------------------------------------------------------------------------------
void PlaybackRateModulatorAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
	std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

	if (!xmlState.get())
		return;

	if (!xmlState->hasTagName(m_params.state.getType()))
		return;

	m_params.replaceState(juce::ValueTree::fromXml(*xmlState));
}

//------------------------------------------------------------------------------
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
	return new PlaybackRateModulatorAudioProcessor();
}
