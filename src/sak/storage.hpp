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

#ifndef SAK_STORAGE_H
#define SAK_STORAGE_H

/// @file storage.h
/// This implementation was inspired by the boost::asio buffer.h as
/// a way of providing an abstraction for storage / buffers.

#include <stdint.h>

namespace sak
{

    /// The mutable storage class contains a pointer
    /// and size of a modifiable/mutable buffer
    struct mutable_storage
    {
        /// The storage type
        typedef uint8_t value_type;
        typedef value_type* value_ptr;

        /// Create an empty storage object
        mutable_storage()
            : m_data(0),
              m_size(0)
            { }

        /// Create an initialized mutable storage object
        /// @param size the size of the buffer in bytes
        /// @param data pointer to the storage buffer
        mutable_storage(value_ptr data, uint32_t size)
            : m_data(data),
              m_size(size)
            {
                assert(m_data != 0);
                assert(m_size > 0);
            }

        /// Pointer to the mutable buffer storage
        value_ptr m_data;

        /// The size of the mutable buffer
        uint32_t m_size;

    };


    /// The const storage class contains a pointer and
    /// size of a non-modifiable/const buffer
    struct const_storage
    {

        /// The storage type
        typedef uint8_t value_type;
        typedef const value_type* value_ptr;

        /// Create an empty storage object
        const_storage()
            : m_data(0),
              m_size(0)
            { }

        /// Create an initialized const storage object
        /// @param size the size of the buffer in bytes
        /// @param data pointer to the storage buffer
        const_storage(value_ptr data, uint32_t size)
            : m_data(data),
              m_size(size)
            { }

        /// Creates and const storage object from a mutable
        /// @param s the mutable storage object
        const_storage(const mutable_storage &s)
            : m_data(s.m_data),
              m_size(s.m_size)
            { }

        /// Assigns and converts a mutable storage buffer
        /// into a const storage buffer
        /// @param s the mutable storage object
        const_storage& operator=(const mutable_storage &s)
            {
                m_data = s.m_data;
                m_size = s.m_size;
                return *this;
            }

        /// Pointer to the non-mutable buffer storage
        value_ptr m_data;

        /// The size of the mutable buffer
        uint32_t m_size;

    };

    /// An adapter for the mutable storage class to be used
    /// by algorithms accepting multiple storage objects
    struct mutable_storage_list : public mutable_storage
    {

        /// The value type
        typedef mutable_storage value_type;

        /// The iterator type
        typedef const mutable_storage* const_iterator;

        mutable_storage_list(const mutable_storage &storage)
            : mutable_storage(storage)
            {}

        /// @return interator to the first element note in this
        ///         adapter we always only have one element
        ///         @see mutable_storage_list::end()
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
    };

    /// An adapter for the const storage class to be used
    /// by algorithms accepting multiple storage objects
    struct const_storage_list : public const_storage
    {

        /// The value type
        typedef const_storage value_type;

        /// The iterator type
        typedef const const_storage* const_iterator;

        const_storage_list(const const_storage &storage)
            : const_storage(storage)
            {}

        const_storage_list(const mutable_storage_list &storage_list)
            : const_storage(storage_list)
            {}

        /// @return interator to the first element note in this
        ///         adapter we always only have one element
        ///         @see const_storage_list::end()
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

    };

    /// Defines a storage sequence i.e. a storage mapping where
    /// buffers may be in disjoint memory locations
    template<class Storage>
    struct storage_sequence;

    template<>
    struct storage_sequence<const_storage>
    {
        typedef std::vector<const_storage> type;
    };

    template<>
    struct storage_sequence<mutable_storage>
    {
        typedef std::vector<mutable_storage> type;
    };

    /// Typedefs for the sequences
    typedef storage_sequence<const_storage>::type
        const_storage_sequence;

    typedef storage_sequence<mutable_storage>::type
        mutable_storage_sequence;

    /// Splits a continues storage buffer into a sequence of
    /// storage buffers where the continues buffer is split at
    /// a specified number of bytes
    template<class StorageType>
    inline typename storage_sequence<StorageType>::type
    split_storage(const StorageType &storage, uint32_t split)
    {
        typedef typename StorageType::value_ptr
            value_ptr;

        typedef typename storage_sequence<StorageType>::type
            storage_sequence_type;

        uint32_t remaining_size = storage.m_size;
        value_ptr data_offset = storage.m_data;

        storage_sequence_type sequence;

        while(remaining_size > 0)
        {
            uint32_t next_size = std::min(remaining_size, split);

            sequence.push_back(StorageType(next_size, data_offset));

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
    inline uint32_t storage_size(StorageIterator first, StorageIterator last)
    {
        uint32_t size = 0;
        while(first != last)
        {
            size += first->m_size;
            ++first;
        }
        return size;
    }

    /// Zeros the memory pointed to by a mutable storage
    /// object
    /// @param storage the mutable storage buffer
    inline void zero_storage(mutable_storage &storage)
    {
        std::fill_n(storage.m_data, storage.m_size, 0);
    }

    /// Copies the source storage into the destination storage buffer
    inline void copy_storage(const mutable_storage &dest, const const_storage &src)
    {
        assert(dest.m_size > 0);
        assert(dest.m_size >= src.m_size);
        assert(dest.m_data != 0);
        assert(src.m_data != 0);

        std::copy(src.m_data,
                  src.m_data + src.m_size,
                  dest.m_data);
    }

    /// Casts the stored pointer to a different data type
    /// @param s the storage adapter
    /// @return pointer to the requested storage data type
    template<class ValueType>
    inline ValueType* cast_storage(const mutable_storage &s)
    {
        return reinterpret_cast<ValueType*>(s.m_data);
    }

    /// Casts the stored pointer to a different data type
    /// @param s the storage adapter
    /// @return pointer to the requested storage data type
    template<class ValueType>
    inline const ValueType* cast_storage(const const_storage &s)
    {
        return reinterpret_cast<const ValueType*>(s.m_data);
    }

    /// Creates a const storage object
    /// @param v is a std::vector buffer
    /// @return the storage adapter
    template<class PodType, class Allocator>
    inline const_storage storage(const std::vector<PodType, Allocator> &v)
    {
        uint32_t size = v.size() * sizeof(PodType);
        const uint8_t *data = reinterpret_cast<const uint8_t*>(&v[0]);

        return const_storage(data, size);
    }

    /// Creates a mutable storage object
    /// @param v is a std::vector buffer
    /// @return the storage adapter
    template<class PodType, class Allocator>
    inline mutable_storage storage(std::vector<PodType, Allocator> &v)
    {
        uint32_t size = v.size() * sizeof(PodType);
        uint8_t *data = reinterpret_cast<uint8_t*>(&v[0]);

        return mutable_storage(data, size);
    }

    /// Creates a storage list adapter
    /// @param v is a std::vector buffer
    /// @return the storage list adapter
    template<class PodType, class Allocator>
    inline mutable_storage_list storage_list(std::vector<PodType, Allocator> &v)
    {
        return mutable_storage_list(storage(v));
    }

    /// Creates a storage list adapter
    /// @param v is a std::vector buffer
    /// @return the storage list adapter
    template<class PodType, class Allocator>
    inline const_storage_list storage_list(const std::vector<PodType, Allocator> &v)
    {
        return const_storage_list(storage(v));
    }

    /// Storage function for pointers to const data
    /// @param data pointer to the data buffer
    /// @param size_in_bytes the size of data buffer in bytes
    /// @return the storage adapter
    inline const_storage storage(const void *data, uint32_t size_in_bytes)
    {
        const uint8_t *data_ptr = reinterpret_cast<const uint8_t*>(data);
        return const_storage(data_ptr, size_in_bytes);
    }

    /// Storage function for pointers to mutable data
    /// @param data pointer to the data buffer
    /// @param size_in_bytes the size of data buffer in bytes
    /// @return the storage adapter
    inline mutable_storage storage(void *data, uint32_t size_in_bytes)
    {
        uint8_t *data_ptr = reinterpret_cast<uint8_t*>(data);
        return mutable_storage(data_ptr, size_in_bytes);
    }

    /// Storage list function for pointers to const data
    /// @param data pointer to the data buffer
    /// @param size_in_bytes the size of data buffer in bytes
    /// @return the storage list adapter
    inline const_storage_list storage_list(const void *data, uint32_t size_in_bytes)
    {
        return const_storage_list(storage(data, size_in_bytes));
    }

    /// Storage list function for pointers to mutable data
    /// @param data pointer to the data buffer
    /// @param size_in_bytes the size of data buffer in bytes
    /// @return the storage list adapter
    inline mutable_storage_list storage_list(void *data, uint32_t size_in_bytes)
    {
        return mutable_storage_list(storage(data, size_in_bytes));
    }

}

#endif

