// Copyright (c) 2012 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>

namespace sak
{
    /// Checks the platform uses big or little endianess
    /// From test proposed here:
    /// http://stackoverflow.com/questions/1001307/
    inline bool is_big_endian()
    {
        union
        {
            uint32_t i;
            uint8_t c[4];
        } bint = {0x01020304};

        return bint.c[0] == 1;
    }
}