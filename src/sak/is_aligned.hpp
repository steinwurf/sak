// Copyright (c) 2012 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>
#include <cassert>

#include "platform.hpp"

namespace sak
{

    /// Checks whether a pointer is aligned to a specific memory boundary
    inline bool is_aligned(const void* ptr,
                           uint32_t alignment = platform::simd_align)
    {
        // Is power of 2
        assert(alignment != 0 && ((alignment & (alignment - 1)) == 0));

        return (reinterpret_cast<uintptr_t>(ptr) % alignment) == 0;
    }

}
