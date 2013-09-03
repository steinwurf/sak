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

#include <boost/make_shared.hpp>

#include <sak/random_input_stream.hpp>

TEST(TestRandomInputStream, CreateRandomInputStream)
{
    // Test: read from one buffer to another

    srand(static_cast<uint32_t>(time(0)));

    {
        uint32_t buffer_size = 1143;
        auto input_stream = boost::make_shared<sak::random_input_stream>( buffer_size );

        ASSERT_TRUE( input_stream->size() == buffer_size );
        ASSERT_TRUE( input_stream->read_position() == 0 );

        std::vector<uint8_t> buffer_out;

        while ( input_stream->bytes_available() > 0 )
        {
            // Random read (always positive thus + 1)
            uint32_t read_request = (rand() % 100) + 1;

            uint32_t read = std::min(read_request, input_stream->bytes_available());

            std::vector<uint8_t> read_buffer(read, '\0');

            input_stream->read(read_buffer.data(), read );

            buffer_out.insert(buffer_out.end(), read_buffer.begin(), read_buffer.end());
        }

        EXPECT_EQ(buffer_out.size(), buffer_size);
        ASSERT_TRUE(std::equal(buffer_out.begin(), buffer_out.end(),
                               input_stream->data()));
    }
}
