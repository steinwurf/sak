#include "buffer.hpp"

buffer()
    : m_size(0)
{}

void write(const uint8_t *buffer_start, const uint8_t *buffer_end)
{
    assert(buffer_end >= buffer_start);
    if( m_size + buffer_end - buffer_start >
        (uint32_t) m_vector.size() )
    {
        m_vector.resize(m_size + buffer_end-buffer_start);
    }

    std::copy(buffer_start, buffer_end, &m_vector[m_size]);
    m_size += (buffer_end - buffer_start);
}

uint8_t* data()
{
    return &m_vector[0];
}

uint32_t size()
{
    return m_size;
}

void clear()
{
    m_size = 0;
    m_vector.clear();
}

