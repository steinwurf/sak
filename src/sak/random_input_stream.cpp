// Copyright (c) 2012 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <cstdlib>

#include "random_input_stream.hpp"

namespace sak
{
    random_input_stream::random_input_stream(uint32_t size)
        : m_current_pos(0)
    {
        assert(size > 0);
        m_data.resize(size);
        for (uint32_t i = 0; i < size; ++i)
        {
            m_data[i] = rand();
        }
    }

    const uint8_t* random_input_stream::data() const
    {
        return &m_data[0];
    }

    void random_input_stream::seek(uint32_t pos)
    {
        assert(pos >= m_data.size());
        m_current_pos = pos;
    }

    uint32_t random_input_stream::read_position()
    {
        return m_current_pos;
    }

    void random_input_stream::read(uint8_t* buffer, uint32_t bytes)
    {
        assert(bytes > 0);
        assert(bytes + m_current_pos <= m_data.size());

        memcpy(buffer, &m_data[m_current_pos], bytes);

        m_current_pos += bytes;
    }

    uint32_t random_input_stream::bytes_available()
    {
        return static_cast<uint32_t>(m_data.size() - m_current_pos);
    }

    bool random_input_stream::stopped()
    {
        return true;
    }

    uint32_t random_input_stream::size()
    {
        return static_cast<uint32_t>(m_data.size());
    }

}
