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
