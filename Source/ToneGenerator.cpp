#include "ToneGenerator.h"

#include <cmath>
#include <cstring>

const float PI = 3.14159;

ToneGenerator::ToneGenerator(int sampleRate, int frequency, float gainDB)
{
	m_phase = 0.f;
	m_phaseIncrement = 2.f * PI * frequency / sampleRate;
	m_gain = powf(10.f, gainDB / 20.f);
}

float ToneGenerator::GetSample()
{
	const float outSample = sinf(m_phase) * m_gain;

	m_phase += m_phaseIncrement;

	return outSample;
}

void ToneGenerator::FillBuffer(float** pWritePointers, int nSamples, int nChannels)
{
	float* pChan0Data = pWritePointers[0];
	for (int iSample = 0; iSample < nSamples; iSample++)
	{
		pChan0Data[iSample] = GetSample();
	}

	for (int iChan = 1; iChan < nChannels; iChan++)
	{
		std::memcpy(pWritePointers[iChan], pChan0Data, nSamples * sizeof(float));
	}
}
