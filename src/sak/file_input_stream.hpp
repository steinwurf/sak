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

#pragma once

#include <fstream>
#include <string>
#include <cstdint>

#include "error.hpp"
#include "finite_input_stream.hpp"

namespace sak
{

    /// A file input stream for reading local
    /// files. Mainly used for testing purposes.
    class file_input_stream : public finite_input_stream
    {
    public:

        /// Constructor
        file_input_stream();

        /// Constructor that opens the file immediately
        /// @throws boost::system::system_error Thrown on failure.
        /// @param filename the filename
        file_input_stream(const std::string& filename);

        /// Opens the file
        /// @throws boost::system::system_error Thrown on failure.
        /// @param filename the file name
        void open(const std::string& filename);

        /// Opens the file
        /// @param filename the file name
        /// @param ec on error set to indicate the type of error
        void open(const std::string& filename, boost::system::error_code& ec);

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

    private:

        /// The actual file
        std::ifstream m_file;

        /// The size of the file in bytes
        uint32_t m_filesize;
    };

}
