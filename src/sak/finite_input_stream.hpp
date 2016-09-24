// Copyright (c) 2012 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>
#include <memory>

#include "input_stream.hpp"

namespace sak
{
/// The finite input stream is an abstract interface which provides methods
/// useful for finite data streams.
class finite_input_stream : public input_stream
{
public:

    /// pointer to finite input streams
    typedef std::shared_ptr<finite_input_stream> ptr;

    /// Seeks the read position to a certain position in the input stream.
    /// @param pos position to seek to
    virtual void seek(uint32_t pos) = 0;

    /// @return the current position
    virtual uint32_t read_position() = 0;

    /// @return the size the of the input stream
    virtual uint32_t size() = 0;

public: /// From input_stream

    /// @copydoc input_stream::stopped()
    /// Note, a finite_input_stream will always be stopped since
    /// it has a fixed size and no new data will ever be produced.
    bool stopped()
    {
        return true;
    }
};
}
