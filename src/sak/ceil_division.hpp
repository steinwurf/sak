// Copyright (c) 2012 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>
#include <cassert>

namespace sak
{
    /// Ceiling for integer division - ceil(numerator/denominator)
    /// @param numerator the division numerator
    /// @param denominator the division denominator
    /// @return returns the ceiled result of integer division
    inline uint32_t ceil_division(uint32_t numerator, uint32_t denominator)
    {
        assert(denominator > 0);

        if (numerator == 0)
            return 0;

        return ((numerator - 1) / denominator) + 1;
    }
}
