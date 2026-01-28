#pragma once

#include <vector>

class CircularBuffer
{
	using uint = unsigned int;

public:
	CircularBuffer(uint size);
	CircularBuffer() : CircularBuffer(0) {}

	// returns the sample at index i, where 0 is the most recent sample and -1 is the 2nd most recent sample, etc.
	float operator [](int i);

	// returns the size of the buffer
	uint GetSize() { return m_size; }
	// resizes the buffer
	void Resize(uint newSize);
	// writes a single sample to the buffer
	void WriteSample(float sample);
	// writes a given number of samples to the buffer
	void WriteSamples(const float* samples, uint nSamples);

private:
	std::vector<float> m_buffer;
	uint m_size;
	uint m_head;
};