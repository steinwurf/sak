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

#ifndef SAK_ENDIAN_BUFFER_H
#define SAK_ENDIAN_BUFFER_H

#include <cstdint>

namespace sak {

    /// The idea behind the endian_buffer is to provide a simple interface for
    /// writing to and reading from a buffer. All complexity regarding endianess
    /// is encapsulated.
    class endian_buffer {

    public:

        /// Creates an endian buffer on top of a preallocated buffer of the
        /// specified size
        /// @param buffer a pointer to the buffer
        /// @param size the i size of the buffer
        endian_buffer(uint8_t* buffer, uint32_t size);

        /// Writes 8 bits to the buffer
        /// @param v the value to write
        void write_u8(uint8_t v);

        /// Writes 16 bits to the buffer
        /// @param v the value to write
        void write_u16(uint16_t v);

        /// Writes 32 bits to the buffer
        /// @param v the value to write
        void write_u32(uint32_t v);

        /// Writes 64 bits to the buffer
        /// @param v the value to write
        void write_u64(uint64_t v);

        /// Reads 8 bits from the buffer, and moves the write position.
        /// @return the next 8 bits
        uint8_t read_u8();

        /// Reads 16 bits from the buffer, and moves the write position.
        /// @return the next 16 bits
        uint16_t read_u16();

        /// Reads 32 bits from the buffer, and moves the write position.
        /// @return the next 32 bits
        uint32_t read_u32();

        /// Reads 64 bits from the buffer, and moves the write position.
        /// @return the next 64 bits
        uint64_t read_u64();

        /// Gets the size of the buffer
        /// @return the size of the buffer
        uint32_t size() const;

        /// Gets the current read/write position of the buffer
        /// @return the current position
        uint32_t position() const;

    private:

        /// Pointer to the buffer
        uint8_t* m_buffer;

        /// The current position
        uint32_t m_position;

        /// The size of the buffer
        const uint32_t m_size;
    };
}

#endif
