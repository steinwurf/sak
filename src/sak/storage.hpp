// Copyright (c) 2012, Steinwurf ApS
// All rights reserved.
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

/// @file storage.h
/// This implementation was inspired by the boost::asio buffer.h as
/// a way of providing an abstraction for storage / buffers.

#include <cstdint>
#include <cassert>
#include <cstring>

#include <vector>
#include <string>

namespace sak
{

    /// The mutable storage class contains a pointer
    /// and size of a modifiable/mutable buffer
    struct mutable_storage
    {

        /// The value type used by the iterator
        typedef mutable_storage value_type;

        /// The iterator type
        typedef const mutable_storage* const_iterator;

        /// Create an empty storage object
        mutable_storage() :
            m_data(0),
            m_size(0)
        { }

        /// Create an initialized mutable storage object
        /// @param size the size of the buffer in bytes
        /// @param data pointer to the storage buffer
        mutable_storage(uint8_t* data, uint32_t size) :
            m_data(data),
            m_size(size)
        {
            assert(m_data != 0);
            assert(m_size > 0);
        }

        /// @return interator to the first element note in this
        ///         adapter we always only have one element
        const_iterator begin() const
        {
            return this;
        }

        /// @return interator to the end for this adapter we
        ///         always only have one element thus the + 1
        const_iterator end() const
        {
            return this + 1;
        }

        /// Offset the storage
        mutable_storage& operator+=(uint32_t offset)
        {
            assert(offset <= m_size);
            m_size -= offset;
            m_data += offset;
            return *this;
        }

        /// Offset the storage
        mutable_storage operator+(uint32_t offset)
        {
            assert(offset <= m_size);
            mutable_storage storage(m_data + offset, m_size - offset);

            return storage;
        }

        /// Pointer to the mutable buffer storage
        uint8_t* m_data;

        /// The size of the mutable buffer
        uint32_t m_size;

    };


    /// The const storage class contains a pointer and
    /// size of a non-modifiable/const buffer
    struct const_storage
    {

        /// The value type used by iterators
        typedef const_storage value_type;

        /// The iterator type
        typedef const const_storage* const_iterator;

        /// Create an empty storage object
        const_storage() :
            m_data(0),
            m_size(0)
        { }

        /// Create an initialized const storage object
        /// @param size the size of the buffer in bytes
        /// @param data pointer to the storage buffer
        const_storage(const uint8_t* data, uint32_t size) :
            m_data(data),
            m_size(size)
        { }

        /// Creates and const storage object from a mutable
        /// @param s the mutable storage object
        const_storage(const mutable_storage& s) :
            m_data(s.m_data),
            m_size(s.m_size)
        { }

        /// Assigns and converts a mutable storage buffer
        /// into a const storage buffer
        /// @param s the mutable storage object
        const_storage& operator=(const mutable_storage& s)
        {
            m_data = s.m_data;
            m_size = s.m_size;
            return *this;
        }

        /// @return interator to the first element note in this
        ///         adapter we always only have one element
        const_iterator begin() const
        {
            return this;
        }

        /// @return interator to the end for this adapter we
        ///         always only have one element thus the + 1
        const_iterator end() const
        {
            return this + 1;
        }

        /// Offset the storage
        const_storage& operator+=(uint32_t offset)
        {
            assert(offset <= m_size);
            m_size -= offset;
            m_data += offset;
            return *this;
        }

        /// Offset the storage
        const_storage operator+(uint32_t offset)
        {
            assert(offset <= m_size);
            const_storage storage(m_data + offset, m_size - offset);

            return storage;
        }

        /// Pointer to the non-mutable buffer storage
        const uint8_t* m_data;

        /// The size of the mutable buffer
        uint32_t m_size;

    };

    /// Splits a continuous storage buffer into a sequence of
    /// storage buffers where the original buffer is split at
    /// a specified number of bytes
    template<class StorageType>
    inline std::vector<StorageType>
    split_storage(const StorageType& storage, uint32_t split)
    {
        auto remaining_size = storage.m_size;
        auto data_offset = storage.m_data;

        std::vector<StorageType> sequence;

        while (remaining_size > 0)
        {
            uint32_t next_size = std::min(remaining_size, split);

            sequence.push_back(StorageType(data_offset, next_size));

            data_offset += next_size;
            remaining_size -= next_size;
        }

        return sequence;
    }

    /// Returns the size of all the buffers in a storage sequence
    /// @param first iterator to the first storage adapter
    /// @param last iterator to the last storage adapter
    /// @return the size in bytes of the storage adapters
    template<class StorageIterator>
    inline uint32_t storage_size(StorageIterator first,
                                 StorageIterator last)
    {
        uint32_t size = 0;
        while (first != last)
        {
            size += first->m_size;
            ++first;
        }
        return size;
    }

    /// Zeros the memory pointed to by a mutable storage
    /// object
    /// @param storage the mutable storage buffer
    inline void zero_storage(mutable_storage& storage)
    {
        std::fill_n(storage.m_data, storage.m_size, 0);
    }

    /// Copies the source storage into the destination storage buffer
    inline void copy_storage(const mutable_storage& dest,
                             const const_storage& src)
    {
        assert(dest.m_size > 0);
        assert(dest.m_size >= src.m_size);
        assert(dest.m_data != 0);
        assert(src.m_data != 0);

        std::copy(src.m_data, src.m_data + src.m_size, dest.m_data);
    }

    /// Casts the stored pointer to a different data type
    /// @param s the storage adapter
    /// @return pointer to the requested storage data type
    template<class ValueType>
    inline ValueType* cast_storage(const mutable_storage& s)
    {
        return reinterpret_cast<ValueType*>(s.m_data);
    }

    /// Casts the stored pointer to a different data type
    /// @param s the storage adapter
    /// @return pointer to the requested storage data type
    template<class ValueType>
    inline const ValueType* cast_storage(const const_storage& s)
    {
        return reinterpret_cast<const ValueType*>(s.m_data);
    }



    /// Storage function for pointers to const data
    /// @param data pointer to the data buffer
    /// @param size_in_bytes the size of data buffer in bytes
    /// @return the storage adapter
    inline const_storage storage(const void* data, uint32_t size_in_bytes)
    {
        const uint8_t* data_ptr = reinterpret_cast<const uint8_t*>(data);
        return const_storage(data_ptr, size_in_bytes);
    }

    /// Storage function for pointers to mutable data
    /// @param data pointer to the data buffer
    /// @param size_in_bytes the size of data buffer in bytes
    /// @return the storage adapter
    inline mutable_storage storage(void* data, uint32_t size_in_bytes)
    {
        uint8_t* data_ptr = reinterpret_cast<uint8_t*>(data);
        return mutable_storage(data_ptr, size_in_bytes);
    }


    /// Creates a const storage object
    /// @param v is a std::vector buffer
    /// @return the storage adapter
    template<class PodType, class Allocator>
    inline const_storage storage(const std::vector<PodType, Allocator>& v)
    {
        uint32_t size = uint32_t(v.size() * sizeof(PodType));
        const uint8_t* data = reinterpret_cast<const uint8_t*>(&v[0]);

        return const_storage(data, size);
    }

    /// Creates a mutable storage object
    /// @param v is a std::vector buffer
    /// @return the storage adapter
    template<class PodType, class Allocator>
    inline mutable_storage storage(std::vector<PodType, Allocator>& v)
    {
        uint32_t size = static_cast<uint32_t>(v.size() * sizeof(PodType));
        uint8_t* data = reinterpret_cast<uint8_t*>(&v[0]);

        return mutable_storage(data, size);
    }


    /// Creates a const storage object from a string
    /// @param str is a std::string
    /// @return the storage adapter
    inline const_storage storage(const std::string& str)
    {
        uint32_t size = (uint32_t)str.size();
        const uint8_t* data = reinterpret_cast<const uint8_t*>(&str[0]);

        return const_storage(data, size);
    }

    /// Creates a mutable storage object from a string
    /// @param str is a std::string
    /// @return the storage adapter
    inline mutable_storage storage(std::string& str)
    {
        uint32_t size = (uint32_t)str.size();
        uint8_t* data = reinterpret_cast<uint8_t*>(&str[0]);

        return mutable_storage(data, size);
    }

    /// Compares two storage objects to see whether they contain
    /// the same data
    /// @param storage_a The first storage object
    /// @param storage_b The second storage object
    /// @return True if the storage objects contain the same data
    ///         otherwise false.
    inline bool equal(const const_storage& storage_a,
                      const const_storage& storage_b)
    {

        if (storage_a.m_size != storage_b.m_size)
        {
            return false;
        }

        // They have the same size - do they point to the same data?

        if (storage_a.m_data == storage_b.m_data)
        {
            return true;
        }

        // It is two different buffers - is the content equal?
        auto zero_is_equal = std::memcmp(storage_a.m_data,
                                         storage_b.m_data, storage_b.m_size);
        return zero_is_equal == 0;
    }

}
