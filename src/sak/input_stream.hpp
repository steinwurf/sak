// Copyright (c) 2012, Steinwurf ApS
// All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of Steinwurf ApS nor the
//       names of its contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL Steinwurf ApS BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef SAK_INPUT_STREAM_HPP
#define SAK_INPUT_STREAM_HPP

#include <cstdint>

#include <boost/signals2.hpp>
#include <boost/shared_ptr.hpp>

namespace sak
{
    namespace bs2 = boost::signals2;

    /// Input stream abstraction
    class input_stream
    {
    public:

        /// Destructor
        virtual ~input_stream()
        {}

        /// Request a read from the io device
        virtual void read(uint8_t *buffer, uint32_t bytes) = 0;

        /// Returns the number of bytes available
        /// for reading
        virtual uint32_t bytes_available() = 0;

        /// Returns whether no more data will be produced. E.g. in the case
        /// of an live stream the function will return true once the stream
        /// closes. In the case of a finite stream e.g. a file, the function
        /// will always return true.
        virtual bool stopped() = 0;

    public:

        /// Signal emitted when data can be read
        typedef bs2::signal<void ()> ready_read_signal;

        /// Specify callbacks to allow the caller to determine if
        /// new data has arrived which is ready to be read.
        /// @param slot the slot to connect          *
        bs2::connection on_ready_read(const ready_read_signal::slot_type &slot)
        {
            return m_ready_read_signal.connect(slot);
        }

        /// Signal emitted on error
        typedef bs2::signal<void (const std::string&)> error_signal;

        /// Connect to the error signal
        /// @param slot, the function to call
        bs2::connection on_error(const error_signal::slot_type &slot)
        {
            return m_error_signal.connect(slot);
        }

        /// Signal emitted when the stream is stopped
        typedef bs2::signal<void ()> stopped_signal;


        /// Connect to the stopped signal
        /// @param slot, the function to call
        bs2::connection on_stopped(const stopped_signal::slot_type &slot)
        {
            return m_stopped_signal.connect(slot);
        }

    protected:

        /// The ready read signal
        ready_read_signal m_ready_read_signal;

        /// The error signal
        error_signal m_error_signal;

        /// The stopped signals
        stopped_signal m_stopped_signal;

    };

}

#endif
