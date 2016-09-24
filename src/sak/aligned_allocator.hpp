// Copyright (c) 2012 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <algorithm>
#include <cstdint>
#include <cassert>
#include <limits>
#include <type_traits>

namespace sak
{

/// The aligned allocator should provide memory allocation for STL
/// containers which are aligned on a specific byte boundary.
///
/// Our use-case for this allocator is to ensure that if used with
/// a std::vector then the buffer will start at the specified byte
/// boundary.
///
/// The allocator may be used with a std::vector like this:
///
/// std::vector<uint8_t, sak::aligned_allocator<uint8_t> > vector;
///
/// Defaults to 32bit alignment, the lowest common denominotor on the
/// supported platforms.
///
/// The allocator is based on the code example from:
/// The C++ Standard Library - A Tutorial and Reference
/// by Nicolai M. Josuttis, Addison-Wesley, 1999
template<class T, uint32_t Alignment = 32>
class aligned_allocator
{
public:

    static_assert(Alignment != 0 && ((Alignment & (Alignment - 1)) == 0),
                  "Must be a power of 2");

public:

    /// Type definitions
    typedef T        value_type;
    typedef T*       pointer;
    typedef const T* const_pointer;
    typedef T&       reference;
    typedef const T& const_reference;
    typedef std::size_t    size_type;
    typedef std::ptrdiff_t difference_type;

    /// Rebind allocator to type U
    template <class U>
    struct rebind
    {
        typedef aligned_allocator<U> other;
    };

    /// @return address of values
    pointer address(reference value) const
    {
        return &value;
    }

    const_pointer address(const_reference value) const
    {
        return &value;
    }

    /// @return the alignment of the internal buffer
    uint32_t alignment() const
    {
        return Alignment;
    }

    /// Constructors the allocator has no state
    aligned_allocator()
    { }

    aligned_allocator(const aligned_allocator&)
    { }

    template <class U>
    aligned_allocator(const aligned_allocator<U>&)
    { }

    /// Destructor the allocator has no state
    ~aligned_allocator()
    { }

    /// @return maximum number of elements that can be allocated
    size_type max_size() const
    {
        return std::numeric_limits<std::size_t>::max() / sizeof(T);
    }

    /// Allocate but don't initialize num elements of type T
    ///
    /// We want to make sure that the data is correctly aligned
    ///
    ///     |  ..... unused ...... | .. offset .. | ..... real data ...... |
    ///     ^                                     ^
    ///       old_ptr                               new_ptr
    ///
    ///       Unused:               Offset:         Real data:
    ///       Address returned      Difference      Address returned by us
    ///       by the memory         between the     which is aligned to the
    ///       allocator             old_ptr and     right memory boundary
    ///                             new_ptr
    ///
    /// The offset is stored one byte below the new_ptr to make sure that we
    /// can find the old_ptr again when we need to deallocate the memory
    pointer allocate(size_type num, const void* = 0)
    {
        uint32_t space_needed =
            static_cast<uint32_t>(num * sizeof(T)) + Alignment;

        uint8_t* old_ptr =
            reinterpret_cast<uint8_t*>(::operator new(space_needed));

        uint8_t* new_ptr = find_aligned(old_ptr);

        uint32_t offset = static_cast<uint32_t>(new_ptr - old_ptr);

        assert(offset > 0);
        assert(offset <= Alignment);
        assert(offset <= std::numeric_limits<uint8_t>::max());

        // Store the offset to the old_ptr so that we can find it again
        // when we need to deallocate
        new_ptr[-1] = offset;

        return reinterpret_cast<pointer>(new_ptr);
    }

    /// Given the pointer ptr we find the next location where we have an
    /// address with the desired alignment
    /// @param ptr the start address
    /// @return the next aligned address
    uint8_t* find_aligned(uint8_t* ptr)
    {
        uintptr_t p = reinterpret_cast<uintptr_t>(ptr);
        return ptr + (Alignment - (p % Alignment));
    }

    template<class U, class... Args>
    void construct(U* p, Args&& ... args)
    {
        ::new(static_cast<void*>(p)) U(std::forward<Args>(args)...);
    }

    /// destroy elements of initialized storage p
    void destroy(pointer p)
    {
        (void) p; // suppress unused param warning on msvc
        // destroy objects by calling their destructor
        p->~T();
    }

    /// deallocate storage p of deleted elements
    void deallocate(pointer p, size_type /*num*/)
    {
        uint8_t* new_ptr = reinterpret_cast<uint8_t*>(p);
        uint32_t offset = new_ptr[-1];
        uint8_t* old_ptr = new_ptr - offset;

        ::operator delete(reinterpret_cast<void*>(old_ptr));
    }
};

/// @return that all specializations of this allocator are interchangeable
template <class T1, class T2>
bool operator==(const aligned_allocator<T1>&, const aligned_allocator<T2>&)
{
    return true;
}

template <class T1, class T2>
bool operator!=(const aligned_allocator<T1>&, const aligned_allocator<T2>&)
{
    return false;
}

}
