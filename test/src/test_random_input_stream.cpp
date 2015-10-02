// Copyright (c) 2012 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <sak/random_input_stream.hpp>

#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <memory>
#include <vector>

#include <gtest/gtest.h>

TEST(TestRandomInputStream, CreateRandomInputStream)
{
    // Test: read from one buffer to another
    srand(static_cast<uint32_t>(time(0)));

    uint32_t buffer_size = 1143;
    sak::random_input_stream stream(buffer_size);

    EXPECT_EQ(buffer_size, stream.size());
    EXPECT_EQ(buffer_size, stream.bytes_available());
    EXPECT_EQ(0U, stream.read_position());
    EXPECT_TRUE(stream.stopped());

    stream.seek(99);
    EXPECT_EQ(99U, stream.read_position());
    EXPECT_EQ(buffer_size - 99U, stream.bytes_available());

    stream.seek(0);
    EXPECT_EQ(0U, stream.read_position());
    EXPECT_EQ(buffer_size, stream.bytes_available());

    std::vector<uint8_t> buffer_out;

    while (stream.bytes_available() > 0)
    {
        // Random read (always positive, so we need +1)
        uint32_t read_request = (rand() % 100) + 1;

        uint32_t read = std::min(read_request, stream.bytes_available());

        std::vector<uint8_t> read_buffer(read, '\0');

        stream.read(read_buffer.data(), read);

        buffer_out.insert(buffer_out.end(), read_buffer.begin(),
                          read_buffer.end());
    }

    EXPECT_EQ(buffer_out.size(), buffer_size);
    EXPECT_TRUE(std::equal(buffer_out.begin(), buffer_out.end(),
                           stream.data()));
}
