// Copyright (c) 2012 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include "buffer_input_stream.hpp"

namespace sak
{

    buffer_input_stream::buffer_input_stream(const const_storage& buffer_storage)
        : m_buffer_storage(buffer_storage),
          m_current_pos(0)
    {
        assert(m_buffer_storage.m_size > 0);
        assert(m_buffer_storage.m_data != 0);
    }


    void buffer_input_stream::seek(uint32_t pos)
    {
        assert(pos <= m_buffer_storage.m_size);
        m_current_pos = pos;
    }

    uint32_t buffer_input_stream::read_position()
    {
        return m_current_pos;
    }

    void buffer_input_stream::read(uint8_t* buffer, uint32_t bytes)
    {
        assert(bytes > 0);
        assert(buffer != 0);
        assert(bytes + m_current_pos <= m_buffer_storage.m_size);

        memcpy(buffer, m_buffer_storage.m_data + m_current_pos, bytes);

        m_current_pos += bytes;
    }

    uint32_t buffer_input_stream::bytes_available()
    {
        return m_buffer_storage.m_size - m_current_pos;
    }

    bool buffer_input_stream::stopped()
    {
        return true;
    }

    uint32_t buffer_input_stream::size()
    {
        return m_buffer_storage.m_size;
    }

}
