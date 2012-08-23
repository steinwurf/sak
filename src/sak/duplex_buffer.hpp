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

#ifndef SAK_DUPLEX_BUFFER_HPP
#define SAK_DUPLEX_BUFFER_HPP

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
        duplex_buffer(const duplex_buffer &buffer);

        /// Initializes the buffer from an existing buffer
        /// @param buffer an existing buffer
        duplex_buffer& operator=(const duplex_buffer &buffer);

        /// Swaps the content of two duplex_buffer objects.
        /// @param buffer the target buffer for the swap
        void swap(duplex_buffer &buffer);

        /// @return pointer to the data
        uint8_t* data();

        /// @return pointer to the data
        const uint8_t* data() const;

        /// @return the size of the buffer in bytes
        uint32_t size() const;

        /// Re-sizes the front of the buffer
        /// @param size the new size of the buffer
        void resize_front(uint32_t size);

        /// Re-sizes the back of the buffer
        /// @param size the new size of the buffer
        void resize_back(uint32_t size);

    private:

        /// @param size the number of bytes to shrink the buffer
        ///        from the front i.e. subtract 4
        ///        Before: | ...... | ... data ... | ...... |
        ///        After:  | .......... | . data . | ...... |
        void subtract_from_front(uint32_t size);

        /// @param size the number of bytes to grow the buffer
        ///        from the front i.e. add 4
        ///        Before: | ...... | ... data ... | ...... |
        ///        After:  | .. | ..... data ..... | ...... |
        void add_to_front(uint32_t size);

        /// @param size the number of bytes to shrink the buffer
        ///        from the back i.e. subtract 4
        ///        Before: | ...... | ... data ... | ...... |
        ///        After:  | ...... | . data . | .......... |
        void subtract_from_back(uint32_t size);

        /// @param size the number of bytes to grow the buffer from
        ///        from the back i.e. add 4
        ///        Before: | ...... | ... data ... | ...... |
        ///        After:  | ...... | ..... data ..... | .. |
        void add_to_back(uint32_t size);

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

    inline duplex_buffer::duplex_buffer(uint32_t size)
        : m_buffer(size),
          m_front_capacity(0),
          m_back_capacity(0),
          m_data_size(size)
    { }

    inline duplex_buffer::duplex_buffer(uint32_t size,
                                 uint32_t front_capacity,
                                 uint32_t back_capacity)
        : m_buffer(size + front_capacity + back_capacity),
          m_front_capacity(front_capacity),
          m_back_capacity(back_capacity),
          m_data_size(size)
    { }

    inline duplex_buffer::duplex_buffer(const duplex_buffer &buffer)
        : m_buffer(buffer.m_buffer),
          m_front_capacity(buffer.m_front_capacity),
          m_back_capacity(buffer.m_back_capacity),
          m_data_size(buffer.m_data_size)
    { }

    inline duplex_buffer& duplex_buffer::operator=(const duplex_buffer &buffer)
    {
        duplex_buffer temp(buffer);
        swap(temp);
        return *this;
    }

    inline void duplex_buffer::swap(duplex_buffer &buffer)
    {
        std::swap(buffer.m_buffer, m_buffer);
        std::swap(buffer.m_front_capacity, m_front_capacity);
        std::swap(buffer.m_back_capacity, m_back_capacity);
        std::swap(buffer.m_data_size, m_data_size);
    }

    inline uint8_t* duplex_buffer::data()
    {
        return &m_buffer[0] + m_front_capacity;
    }

    inline const uint8_t* duplex_buffer::data() const
    {
        return &m_buffer[0] + m_front_capacity;
    }

    inline void duplex_buffer::resize_front(uint32_t size)
    {
        if(size < m_data_size)
        {
            subtract_from_front(m_data_size - size);
        }
        else if(size > m_data_size)
        {
            add_to_front(size - m_data_size);
        }
    }

    inline void duplex_buffer::resize_back(uint32_t size)
    {
        if(size < m_data_size)
        {
            subtract_from_back(m_data_size - size);
        }
        else if(size > m_data_size)
        {
            add_to_back(size - m_data_size);
        }
    }

    inline uint32_t duplex_buffer::size() const
    {
        return m_data_size;
    }

    inline void duplex_buffer::subtract_from_front(uint32_t size)
    {
        assert(size <= m_data_size);

        // Buffer is shrinking. This is done by
        // moving forward the front pointer right
        m_front_capacity += size;
        m_data_size -= size;
    }


    inline void duplex_buffer::add_to_front(uint32_t size)
    {
        if(size > m_front_capacity)
        {
            realloc(size, m_back_capacity);
        }

        // Buffer is growing. This is done by
        // moving the front pointer left
        m_front_capacity -= size;
        m_data_size += size;
    }

    inline void duplex_buffer::subtract_from_back(uint32_t size)
    {
        assert(size <= m_data_size);

        // Buffer is shrinking. This is done by
        // moving forward the back pointer left
        m_back_capacity += size;
        m_data_size -= size;
    }

    inline void duplex_buffer::add_to_back(uint32_t size)
    {
        if(size > m_back_capacity)
        {
            realloc(m_front_capacity, size);
        }

        // Buffer is growing. This is done by
        // moving the back pointer right
        m_back_capacity -= size;
        m_data_size += size;
    }

    void duplex_buffer::realloc(uint32_t front_capacity, uint32_t back_capacity)
    {
        uint32_t total_size = front_capacity + m_data_size + back_capacity;

        std::vector<uint8_t> buffer(total_size);

        std::copy(&m_buffer[0] + m_front_capacity,
                  &m_buffer[0] + m_front_capacity + m_data_size,
                  &buffer[0] + front_capacity);

        m_buffer.swap(buffer);

        m_front_capacity = front_capacity;
        m_back_capacity  = back_capacity;
    }

}

#endif

