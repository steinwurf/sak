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

#pragma once

#include <cstdint>
#include <vector>

namespace sak
{

    /// The idea behind the duplex_buffer is to support both
    /// efficient prepending and appending of data i.e. writing
    /// to both the front and the back of the buffer.
    ///
    /// This can be illustrated like this:
    ///     | ............. | ... data ... | ............. |
    ///             ^                             ^
    ///        front unused                   back unused
    ///          capacity                       capacity
    ///
    /// Using the resize_front() or resize_back() functions the data
    /// region may be increased or reduced taking space from
    /// either the front or back of the buffer
    class duplex_buffer
    {
    public:

        /// Create a new duplex_buffer of the specified size
        /// no extra capacity is reserved at either the front
        /// or back of the buffer
        /// @param size the size of the buffer in bytes
        explicit duplex_buffer(uint32_t size = 0);

        /// Creates a new duplex_buffer of the specified size and
        /// with the extra capacity reserved at the front and back
        /// of the buffer
        /// @param size the size of the buffer in bytes
        /// @param front_capacity the number of bytes to reserve
        ///        at the front of the buffer
        /// @param back_capacity the number of bytes to reserve
        ///        at the back of the buffer
        duplex_buffer(uint32_t size, uint32_t front_capacity,
                      uint32_t back_capacity);

        /// Creates a new buffer from an existing buffer
        /// @param buffer an existing buffer
        duplex_buffer(const duplex_buffer& buffer);

        /// Initializes the buffer from an existing buffer
        /// @param buffer an existing buffer
        duplex_buffer& operator=(const duplex_buffer& buffer);

        /// Swaps the content of two duplex_buffer objects.
        /// @param buffer the target buffer for the swap
        void swap(duplex_buffer& buffer);

        /// @return pointer to the data
        uint8_t* data();

        /// @return pointer to the data
        const uint8_t* data() const;

        /// @return the size of the buffer in bytes
        uint32_t size() const;

        /// @return pointer to the front of the data buffer corresponds
        ///         to the data() pointer
        uint8_t* front();

        /// @return pointer to the front of the data buffer corresponds
        ///         to the data() pointer
        const uint8_t* front() const;

        /// @return pointer to the front of the data buffer corresponds
        ///         to the data() + size() pointer
        uint8_t* back();

        /// @return pointer to the front of the data buffer corresponds
        ///         to the data() + size() pointer
        const uint8_t* back() const;

        /// Re-sizes the front of the buffer
        /// @param size the new size of the buffer
        void resize_front(uint32_t size);

        /// Re-sizes the back of the buffer
        /// @param size the new size of the buffer
        void resize_back(uint32_t size);

        /// Re-sizes the buffer to the specified size with a minimum front
        /// and back capacity.
        /// @param size the buffer size
        /// @param min_front_capacity the minimum size to reserve for the front
        ///        of the buffer
        /// @param min_back_capacity the minimum size to reserve for the back
        ///        of the buffer
        void resize(uint32_t size,
                    uint32_t min_front_capacity,
                    uint32_t min_back_capacity);

    private:

        /// @param size the number of bytes to shrink the buffer
        ///        from the front i.e. subtract 4
        ///        Before: | ...... | ... data ... | ...... |
        ///        After:  | .......... | . data . | ...... |
        void shrink_front(uint32_t size);

        /// @param size the number of bytes to expand the buffer
        ///        from the front i.e. add 4
        ///        Before: | ...... | ... data ... | ...... |
        ///        After:  | .. | ..... data ..... | ...... |
        void expand_front(uint32_t size);

        /// @param size the number of bytes to shrink the buffer
        ///        from the back i.e. subtract 4
        ///        Before: | ...... | ... data ... | ...... |
        ///        After:  | ...... | . data . | .......... |
        void shrink_back(uint32_t size);

        /// @param size the number of bytes to expand the buffer from
        ///        from the back i.e. add 4
        ///        Before: | ...... | ... data ... | ...... |
        ///        After:  | ...... | ..... data ..... | .. |
        void expand_back(uint32_t size);

        /// Reallocates the buffer making sure that the front and
        /// back capacities are as specified
        /// @param front_capacity the space to reserve at the front
        /// @param back_capacity the space to reserve at the back
        void realloc(uint32_t front_capacity, uint32_t back_capacity);

    private:

        /// The internal buffer
        std::vector<uint8_t> m_buffer;

        /// The size in bytes available
        uint32_t m_front_capacity;
        uint32_t m_back_capacity;

        /// The size in bytes of the data stored in the
        /// buffer
        uint32_t m_data_size;
    };
}
