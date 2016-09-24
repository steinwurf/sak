// Copyright (c) 2012 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>

#include "buffer.hpp"

namespace sak
{
/// Random buffer is meant for testing purposes.
/// Fills a buffer with random data that can be verified later.
class random_buffer : public buffer
{
public:

    /// Resizes the buffer and fills it with random data
    /// The random seed can also be embedded at the beginning of the buffer
    /// @param size the number of bytes to fill
    /// @param embed_seed indicates if the random seed should be embedded
    void fill(uint32_t size, bool embed_seed);

    /// Returns true if the contents of the random buffer can be generated
    /// from the embedded random seed
    bool verify();
};
}
