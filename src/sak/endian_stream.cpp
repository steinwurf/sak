// Copyright (c) 2012 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include "endian_stream.hpp"

namespace sak
{

endian_stream::endian_stream(uint8_t* buffer, uint32_t size) :
    m_buffer(buffer), m_size(size), m_position(0)
{
    assert(m_buffer != 0);
    assert(m_size);
}

endian_stream::endian_stream(const mutable_storage& storage) :
    m_buffer(storage.m_data), m_size(storage.m_size), m_position(0)
{
    assert(m_buffer != 0);
    assert(m_size);
}

uint32_t endian_stream::size() const
{
    return m_size;
}

uint32_t endian_stream::position() const
{
    return m_position;
}

void endian_stream::seek(uint32_t new_position)
{
    assert(new_position <= m_size);
    m_position = new_position;
}

}
