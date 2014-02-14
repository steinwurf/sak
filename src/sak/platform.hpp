// Copyright (c) 2012 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>

namespace sak
{

    /// The idea behind the platform class is to provide access to
    /// information about the run-time platform. For example what
    /// is the capabilities of the CPU etc.
    class platform
    {
    public:

        /// The SIMD align boundary this can vary depending
        /// on whether AVX or SSE are used
        static const uint32_t simd_align = 32;

    };
}
