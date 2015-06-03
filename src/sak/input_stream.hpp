// Copyright (c) 2012 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>
#include <functional>

namespace sak
{
    /// Input stream abstraction
    class input_stream
    {
    public:

        /// Destructor
        virtual ~input_stream()
        {}

        /// Request a read from the io device
        /// @param buffer from where to read
        /// @param bytes to read
        virtual void read(uint8_t* buffer, uint32_t bytes) = 0;

        /// Returns the number of bytes available for reading
        /// @return number of bytes available
        virtual uint32_t bytes_available() = 0;

        /// Indicates if more data will be produced. For a live stream,
        /// the function will return true after the stream finishes.
        /// For a finite stream (e.g. a file) the function will always
        /// return true.
        virtual bool stopped() = 0;

    public:

        /// Signal emitted when data can be read
        typedef std::function<void ()> ready_read_callback;

        /// Specify callbacks to allow the caller to determine if
        /// new data has arrived which is ready to be read.
        /// @param slot the slot to connect
        void on_ready_read(const ready_read_callback& callback)
        {
            m_ready_read_callback = callback;
        }

        /// Signal emitted on error
        typedef std::function<void (const std::string&)> error_callback;

        /// Connect to the error signal
        /// @param slot the function to call
        void on_error(const error_callback& callback)
        {
            m_error_callback = callback;
        }

        /// Signal emitted when the stream is stopped
        typedef std::function<void ()> stopped_callback;

        /// Connect to the stopped signal
        /// @param slot the function to call
        void on_stopped(const stopped_callback& callback)
        {
            m_stopped_callback = callback;
        }

    protected:

        /// The ready read signal
        ready_read_callback m_ready_read_callback;

        /// The error signal
        error_callback m_error_callback;

        /// The stopped signals
        stopped_callback m_stopped_callback;
    };
}
