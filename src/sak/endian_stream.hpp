// Copyright (c) 2011-2013 Steinwurf ApS
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

#include <stdint.h>
#include <cassert>
#include <algorithm>

#include "storage.hpp"
#include "convert_endian.hpp"

namespace sak
{

    /// The idea behind the endian_stream is to provide a stream-like interface
    /// for accessing a fixed-size buffer.
    /// All complexity regarding endianness is encapsulated.
    class endian_stream
    {
    public:

        /// Creates an endian stream on top of a pre-allocated buffer of the
        /// specified size
        /// @param buffer a pointer to the buffer
        /// @param size the size of the buffer in bytes
        endian_stream(uint8_t* buffer, uint32_t size);

        /// Creates an endian stream on top of a mutable storage that has
        /// a fixed size
        /// @param storage the mutable storage
        endian_stream(const mutable_storage& storage);

        /// Writes a value of the size of ValueType to the stream
        /// @param value the value to write
        template<class ValueType>
        void write(ValueType value)
        {
            // Make sure there is enough space in the underlying buffer
            assert(m_size >= m_position + sizeof(ValueType));
            // Write the value at the current position
            big_endian::put<ValueType>(value, &m_buffer[m_position]);
            // Advance the current position
            m_position += sizeof(ValueType);
        }

        /// Writes the contents of a sak::storage container to the stream.
        /// Note that this function is provided only for convenience and
        /// it does not perform any endian conversions. Furthermore, the length
        /// of the container is not written to the stream.
        /// @param storage the storage to write
        void write(const mutable_storage& storage)
        {
            write(const_storage(storage));
        }

        /// Writes the contents of a sak::storage container to the stream.
        /// Note that this function is provided only for convenience and
        /// it does not perform any endian conversions. Furthermore, the length
        /// of the container is not written to the stream.
        /// @param storage the storage to write
        void write(const const_storage& storage)
        {
            // Make sure there is enough space in the underlying buffer
            assert(m_size >= m_position + storage.m_size);
            // Copy the data to the buffer
            std::copy_n(storage.m_data, storage.m_size, &m_buffer[m_position]);
            // Advance the current position
            m_position += storage.m_size;
        }

        /// Reads from the stream and moves the read position.
        /// @param value reference to the value to be read
        template<class ValueType>
        void read(ValueType& value)
        {
            // Make sure there is enough data to read in the underlying buffer
            assert(m_size >= m_position + sizeof(ValueType));
            // Read the value at the current position
            value = big_endian::get<ValueType>(&m_buffer[m_position]);
            // Advance the current position
            m_position += sizeof(ValueType);
        }

        /// Reads data from the stream to fill a mutable storage
        /// Note that this function is provided only for convenience and
        /// it does not perform any endian conversions. Furthermore, the length
        /// of the storage is not read from the stream. Therefore, the
        /// mutable storage must resized before it can be filled.
        /// @param storage the storage to be filled
        void read(const mutable_storage& storage)
        {
            // Make sure there is enough data to read in the underlying buffer
            assert(m_size >= m_position + storage.m_size);
            // Copy the data from the buffer to the storage
            std::copy_n(&m_buffer[m_position], storage.m_size, storage.m_data);
            // Advance the current position
            m_position += storage.m_size;
        }

        /// Gets the size of the underlying buffer
        /// @return the size of the buffer
        uint32_t size() const;

        /// Gets the current read/write position in the stream
        /// @return the current position
        uint32_t position() const;

        /// Changes the current read/write position in the stream
        /// @param new_position the new position
        void seek(uint32_t new_position);

    private:

        /// Pointer to the buffer
        uint8_t* m_buffer;

        /// The size of the buffer
        uint32_t m_size;

        /// The current position
        uint32_t m_position;

    };

}
