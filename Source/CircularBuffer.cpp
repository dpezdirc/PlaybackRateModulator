#include "CircularBuffer.h"

#include <algorithm>
#include <assert.h>

CircularBuffer::CircularBuffer(uint size) :
	m_size(size),
	m_head(0)
{
	m_buffer.resize(size);
}

float CircularBuffer::operator[](int i)
{
	assert(i < 1 && i > -(int)m_size);

	int idx = m_head - 1 + i;
	if (idx < 0)
		idx = idx + m_size;

	return m_buffer[idx];
}

void CircularBuffer::Resize(uint newSize)
{
	if (newSize == m_size)
		return;

	const uint dSize = abs((int)newSize - (int)m_size);

	// shrink: erase the last {dSize} elements of the buffer
	if (newSize < m_size)
	{
		int idxToCopyFrom = m_head + dSize;
		if (idxToCopyFrom > (int)m_size - 1)
			idxToCopyFrom -= m_size;

		const uint idxToCopyTo = std::max(idxToCopyFrom - (int)dSize, 0);
		const uint nElementsToCopy = m_buffer.size() - idxToCopyFrom;

		memcpy(&m_buffer[idxToCopyTo], &m_buffer[idxToCopyFrom], nElementsToCopy * sizeof(float));

		m_buffer.resize(newSize);
		m_head = idxToCopyTo;
	}
	// grow: insert zeros at head position
	else
	{
		m_buffer.resize(newSize);

		const uint nElementsToCopy = m_size - m_head;

		memmove(&m_buffer[m_head] + dSize, &m_buffer[m_head], nElementsToCopy * sizeof(float));
		memset(&m_buffer[m_head], 0, dSize * sizeof(float));
	}

	m_size = newSize;
}

void CircularBuffer::WriteSample(float sample)
{
	m_buffer[m_head] = sample;
	//m_head = (m_head + 1) % m_size;
	if (++m_head >= m_buffer.size())
		m_head = 0; // m_buffer.size();
}

void CircularBuffer::WriteSamples(const float* samples, uint nSamples)
{
	for (uint i = 0; i < nSamples; i++)
	{
		WriteSample(samples[i]);
	}
}
