// Copyright (c) 2011-2012 Steinwurf ApS
// All Rights Reserved

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of Steinwurf ApS nor the
//       names of its contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL Steinwurf ApS BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "endian_buffer.hpp"

#include "convert_endian.h"

#include <cassert>

namespace sak
{
    endian_buffer::endian_buffer(uint8_t* buffer, uint32_t size)
        : m_buffer(buffer),
          m_position(0),
          m_size(size)
    {
        assert(m_buffer != 0);
        assert(m_size);
    }

    void endian_buffer::write_u8(uint8_t v)
    {
        uint8_t* write_position = m_buffer+m_position;
        m_position += sizeof(uint8_t);
        assert(m_position <= m_size);
        big_endian::put<uint8_t>(v, write_position);
    }

    void endian_buffer::write_u16(uint16_t v)
    {
        uint8_t* write_position = m_buffer+m_position;
        m_position += sizeof(uint16_t);
        assert(m_position <= m_size);
        big_endian::put<uint16_t>(v, write_position);

    }

    void endian_buffer::write_u32(uint32_t v)
    {
        uint8_t* write_position = m_buffer+m_position;
        m_position += sizeof(uint32_t);
        assert(m_position <= m_size);
        big_endian::put<uint32_t>(v, write_position);

    }

    void endian_buffer::write_u64(uint64_t v)
    {
        uint8_t* write_position = m_buffer+m_position;
        m_position += sizeof(uint64_t);
        assert(m_position <= m_size);
        big_endian::put<uint64_t>(v, write_position);

    }

    uint8_t endian_buffer::read_u8()
    {
        m_position -= sizeof(uint8_t);
        return big_endian::get<uint8_t>(m_buffer + m_position);
    }

    uint16_t endian_buffer::read_u16()
    {
        m_position -= sizeof(uint16_t);
        return big_endian::get<uint16_t>(m_buffer +  m_position);
    }

    uint32_t endian_buffer::read_u32()
    {
        m_position -= sizeof(uint32_t);
        return big_endian::get<uint32_t>(m_buffer + m_position);
    }

    uint64_t endian_buffer::read_u64()
    {
        m_position -= sizeof(uint64_t);
        return big_endian::get<uint64_t>(m_buffer + m_position);
    }

    uint32_t endian_buffer::size() const
    {
        return m_size;
    }

    uint32_t endian_buffer::position() const
    {
        return m_position;
    }
}
