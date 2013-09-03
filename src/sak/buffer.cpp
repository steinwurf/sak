// Copyright (c) 2012 Steinwurf ApS
// All Rights Reserved
//
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
//
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

#include "buffer.hpp"

#include <algorithm>

namespace sak
{

    buffer::buffer()
        : m_size(0)
    { }

    buffer::buffer(uint32_t capacity)
        : m_size(0)
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
