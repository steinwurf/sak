// Copyright (c) 2012 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <vector>
#include <stdint.h>

#include <boost/type_traits/is_pod.hpp>
#include <boost/type_traits/is_pointer.hpp>
#include <boost/static_assert.hpp>

#include "finite_input_stream.hpp"
#include "storage.hpp"

namespace sak
{

    /// Buffer input stream wrapping a memory buffer. Note that the
    /// memory must be kept "alive" as long as the buffer input stream
    /// is in use.
    class buffer_input_stream : public finite_input_stream
    {
    public:

        /// Constructor
        /// @param buffer_storage will be used as a data storage
        buffer_input_stream(const const_storage& buffer_storage);

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

        /// @copydoc input_stream::stopped()
        bool stopped();

    protected:

        /// The buffer storage
        const_storage m_buffer_storage;

        /// The current read position
        uint32_t m_current_pos;

    };

}