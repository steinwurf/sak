// Copyright (c) 2012-2013, Steinwurf ApS
// All rights reserved.
//
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
//
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

#include <cstdint>
#include <ctime>

#include <gtest/gtest.h>

#include <sak/buffer_input_stream.hpp>

TEST(TestBufferInputStream, CreateBufferInputStream)
{
    // Test: read from one buffer to another

    srand(static_cast<uint32_t>(time(0)));

    {
        uint32_t buffer_size = rand() % 100000;

        std::vector<char> buffer(buffer_size, '\0');

        for (uint32_t i = 0; i < buffer_size; ++i)
        {
            buffer[i] = (rand() % 255);
        }

        boost::shared_ptr<sak::buffer_input_stream> input_stream
            = sak::make_buffer_input( buffer );

        ASSERT_TRUE( input_stream->size() == buffer_size );
        ASSERT_TRUE( input_stream->read_position() == 0 );

        std::vector<char> buffer_out;

        while ( input_stream->bytes_available() > 0 )
        {
            // Random read (always positive thus + 1)
            uint32_t read_request = (rand() % 100) + 1;

            uint32_t read = std::min(read_request, input_stream->bytes_available());

            std::vector<char> read_buffer(read, '\0');

            input_stream->read( reinterpret_cast<uint8_t*>(read_buffer.data()), read );

            buffer_out.insert(buffer_out.end(), read_buffer.begin(), read_buffer.end());
        }

        ASSERT_TRUE(std::equal(buffer.begin(), buffer.end(), buffer_out.begin()));

    }

}
