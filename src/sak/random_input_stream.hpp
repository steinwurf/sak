// Copyright (c) 2012 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <vector>
#include <cstdint>
#include <vector>

#include "finite_input_stream.hpp"

namespace sak
{
/// Buffer input stream is meant for testing purposes. Provides an input
/// from a buffer with random data.
class random_input_stream : public finite_input_stream
{
public:

    /// Constructor
    /// @param size the size in bytes of the buffer passed
    random_input_stream(uint32_t size);

    /// @return a pointer to the random data
    const uint8_t* data() const;

public: // From finite_input_stream

    /// @copydoc finite_input_stream::seek()
    void seek(uint32_t pos);

    /// @copydoc finite_input_stream::read_position()
    uint32_t read_position();

    /// @copydoc finite_input_stream::size()
    uint32_t size();

public: // From input_stream

    /// @copydoc input_stream::read()
    void read(uint8_t* buffer, uint32_t bytes);

    /// @copydoc input_stream::bytes_available()
    uint32_t bytes_available();

protected:

    /// Pointer to the buffer
    std::vector<uint8_t> m_data;

    /// The current read position
    uint32_t m_current_pos;
};
}
