#include "endian_buffer.h"

#include "convert_endian.h"

#include <assert.h>

namespace sak
{
    endian_buffer::endian_buffer(uint8_t* buffer, uint32_t size)
        : m_buffer(buffer),
          m_position(0),
          m_size(size)
    {
        assert(m_buffer != 0);
        assert(m_size != 0);
    }

    void endian_buffer::write_u8(uint8_t v)
    {
        big_endian::put<uint8_t>(v, m_buffer+m_position);
        m_position += sizeof(uint8_t);
    }

    void endian_buffer::write_u16(uint16_t v)
    {
        big_endian::put<uint16_t>(v, m_buffer+m_position);
        m_position += sizeof(uint16_t);
    }

    void endian_buffer::write_u32(uint32_t v)
    {
        big_endian::put<uint32_t>(v, m_buffer+m_position);
        m_position += sizeof(uint32_t);
    }

    void endian_buffer::write_u64(uint64_t v)
    {
        big_endian::put<uint64_t>(v, m_buffer+m_position);
        m_position += sizeof(uint64_t);
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

    uint32_t endian_buffer::size()
    {
        return m_size;
    }

    uint32_t endian_buffer::position()
    {
        return m_position;
    }
}
