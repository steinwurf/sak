// Copyright (c) 2012 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

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
