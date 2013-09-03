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

#pragma once

#include <vector>

#include "storage.hpp"

namespace sak
{

    /// Helper class that implements a simple buffer. Data can be written
    /// into the buffer which appends it to the existing data. Keeps
    /// track of the amount of data in the buffer.
    class buffer
    {
    public:

        /// Constructs a new zero sized buffer
        buffer();

        /// Constructs a new buffer with an initial capacity
        buffer(uint32_t capacity);

        /// Appends data to the end of the buffer
        /// @param data the bytes to append to the end of the buffer
        /// @param size the size of the data in bytes
        void append(const uint8_t* data, uint32_t size);

        /// Appends data to the end of the buffer
        /// @param data_start the start address of the of the data
        /// @param data_end the end address of the data
        void append(const uint8_t* data_start, const uint8_t* data_end);

        /// Appends data to the end of the buffer
        /// @param storage the data to append
        template<class Storage>
        void append(const Storage& storage);

        /// @return a pointer to the start of the data in the buffer
        const uint8_t* data() const;

        /// @return a pointer to the start of the data in the buffer
        uint8_t* data();

        /// @return the size in bytes of the data in the buffer
        uint32_t size() const;

        /// Re-sizes the buffer to a specific size
        /// If size is greater than the current size, the buffer is extended
        /// to make it size bytes with the extra bytes added to the end.
        /// The new bytes are uninitialized.
        /// If size is less than the current size, bytes are removed from
        /// the end.
        void resize(uint32_t size);

        /// Sets the buffer to zero size
        void clear();

    private:

        /// Internal storage
        std::vector<uint8_t> m_vector;

        /// The amount on internally stored data
        uint32_t m_size;
    };

    template<class Storage>
    inline void buffer::append(const Storage& storage)
    {
        for (auto it = storage.begin(); it != storage.end(); ++it)
        {
            append(it->m_data, it->m_size);
        }
    }

}
