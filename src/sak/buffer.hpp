// Copyright (c) 2012 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

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
