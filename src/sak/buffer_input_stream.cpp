// Copyright (c) 2012, Steinwurf ApS
// All rights reserved.

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
