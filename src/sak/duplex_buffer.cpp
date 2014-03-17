// Copyright (c) 2012 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include "duplex_buffer.hpp"
#include <cassert>

namespace sak
{

    duplex_buffer::duplex_buffer(uint32_t size)
        : m_buffer(size),
          m_front_capacity(0),
          m_back_capacity(0),
          m_data_size(size)
    { }

    duplex_buffer::duplex_buffer(uint32_t size,
                                 uint32_t front_capacity,
                                 uint32_t back_capacity)
        : m_buffer(size + front_capacity + back_capacity),
          m_front_capacity(front_capacity),
          m_back_capacity(back_capacity),
          m_data_size(size)
    { }

    duplex_buffer::duplex_buffer(const duplex_buffer& buffer)
        : m_buffer(buffer.m_buffer),
          m_front_capacity(buffer.m_front_capacity),
          m_back_capacity(buffer.m_back_capacity),
          m_data_size(buffer.m_data_size)
    { }

    duplex_buffer& duplex_buffer::operator=(const duplex_buffer& buffer)
    {
        duplex_buffer temp(buffer);
        swap(temp);
        return *this;
    }

    void duplex_buffer::swap(duplex_buffer& buffer)
    {
        std::swap(buffer.m_buffer, m_buffer);
        std::swap(buffer.m_front_capacity, m_front_capacity);
        std::swap(buffer.m_back_capacity, m_back_capacity);
        std::swap(buffer.m_data_size, m_data_size);
    }

    uint8_t* duplex_buffer::data()
    {
        return &m_buffer[0] + m_front_capacity;
    }

    const uint8_t* duplex_buffer::data() const
    {
        return &m_buffer[0] + m_front_capacity;
    }

    uint8_t* duplex_buffer::front()
    {
        return data();
    }

    const uint8_t* duplex_buffer::front() const
    {
        return data();
    }

    uint8_t* duplex_buffer::back()
    {
        return data() + size();
    }

    const uint8_t* duplex_buffer::back() const
    {
        return data() + size();
    }

    void duplex_buffer::resize_front(uint32_t size)
    {
        if (size < m_data_size)
        {
            shrink_front(m_data_size - size);
        }
        else if (size > m_data_size)
        {
            expand_front(size - m_data_size);
        }
    }

    void duplex_buffer::resize_back(uint32_t size)
    {
        if (size < m_data_size)
        {
            shrink_back(m_data_size - size);
        }
        else if (size > m_data_size)
        {
            expand_back(size - m_data_size);
        }
    }

    void duplex_buffer::resize(uint32_t size,
                               uint32_t min_front_capacity,
                               uint32_t min_back_capacity)
    {
        uint32_t total_size = min_front_capacity + size + min_back_capacity;

        if (m_buffer.size() < total_size)
        {
            m_buffer.resize(total_size);
        }

        uint32_t extra_space = static_cast<uint32_t>(m_buffer.size() - total_size);
        m_front_capacity = min_front_capacity;
        m_data_size = size;
        m_back_capacity = min_back_capacity + extra_space;
    }

    uint32_t duplex_buffer::size() const
    {
        return m_data_size;
    }

    void duplex_buffer::shrink_front(uint32_t size)
    {
        assert(size <= m_data_size);

        // Buffer is shrinking. This is done by
        // moving forward the front pointer right
        m_front_capacity += size;
        m_data_size -= size;
    }

    void duplex_buffer::expand_front(uint32_t size)
    {
        if (size > m_front_capacity)
        {
            realloc(size, m_back_capacity);
        }

        // Buffer is growing. This is done by
        // moving the front pointer left
        m_front_capacity -= size;
        m_data_size += size;
    }

    void duplex_buffer::shrink_back(uint32_t size)
    {
        assert(size <= m_data_size);

        // Buffer is shrinking. This is done by
        // moving forward the back pointer left
        m_back_capacity += size;
        m_data_size -= size;
    }

    void duplex_buffer::expand_back(uint32_t size)
    {
        if (size > m_back_capacity)
        {
            realloc(m_front_capacity, size);
        }

        // Buffer is growing. This is done by
        // moving the back pointer right
        m_back_capacity -= size;
        m_data_size += size;
    }


    void duplex_buffer::realloc(uint32_t front_capacity, uint32_t back_capacity)
    {
        uint32_t total_size = front_capacity + m_data_size + back_capacity;

        std::vector<uint8_t> buffer(total_size);

        std::copy(&m_buffer[0] + m_front_capacity,
                  &m_buffer[0] + m_front_capacity + m_data_size,
                  &buffer[0] + front_capacity);

        m_buffer.swap(buffer);

        m_front_capacity = front_capacity;
        m_back_capacity = back_capacity;
    }
}
