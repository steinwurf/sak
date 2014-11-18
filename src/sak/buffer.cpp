// Copyright (c) 2012 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include "buffer.hpp"

#include <algorithm>

namespace sak
{
    buffer::buffer() :
        m_size(0)
    { }

    buffer::buffer(uint32_t capacity) :
        m_size(0)
    {
        m_vector.resize(capacity);
    }

    void buffer::append(const uint8_t* data, uint32_t size)
    {
        assert(data != 0);
        assert(size > 0);

        if (m_size + size > m_vector.size())
        {
            m_vector.resize(m_size + size);
        }

        std::copy_n(data, size, &m_vector[m_size]);
        m_size += size;
    }

    void buffer::append(const uint8_t* data_start, const uint8_t* data_end)
    {
        assert(data_start != 0);
        assert(data_end != 0);
        assert(data_start < data_end);

        uint32_t size = static_cast<uint32_t>(data_end - data_start);
        append(data_start, size);
    }

    const uint8_t* buffer::data() const
    {
        return &m_vector[0];
    }

    uint8_t* buffer::data()
    {
        return &m_vector[0];
    }

    uint32_t buffer::size() const
    {
        return m_size;
    }

    void buffer::resize(uint32_t size)
    {
        // Currently our re-allocation strategy is not
        // to "shrink" the buffer. If at some point this
        // should become important in an application
        // we could add that possibility as a reallocation-policy

        if (size <= m_vector.size())
        {
            m_size = size;
        }
        else
        {
            m_vector.resize(size);
            m_size = size;
        }
    }

    void buffer::clear()
    {
        m_size = 0;
    }
}
