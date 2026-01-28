#pragma once

class ToneGenerator
{
public:
	ToneGenerator(int sampleRate, int frequency, float gainDB);
	ToneGenerator() : ToneGenerator(1, 1, 0.f) {}

	float GetSample();
	void FillBuffer(float** pWritePointers, int nSamples, int nChannels);

private:
	float m_phase;
	float m_gain;
	float m_phaseIncrement;
	unsigned int m_frequency;
};