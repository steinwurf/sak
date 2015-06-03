// Copyright (c) 2012 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <sak/random_buffer.hpp>

#include <cstdint>
#include <ctime>

#include <gtest/gtest.h>

void test_random_buffer(uint32_t buffer_size)
{
    sak::random_buffer buffer;

    // Fill the buffer with random data and embed the random seed
    buffer.fill(buffer_size, true);
    ASSERT_EQ(buffer.size(), buffer_size);

    // Verify the contents of the buffer using the embedded random seed
    bool verified = buffer.verify();
    EXPECT_TRUE(verified);

    // Change the last byte of the random data
    uint8_t* last_byte = buffer.data() + buffer_size - 1;
    *last_byte = *last_byte + 1;
    // The verification should fail after this tampering
    verified = buffer.verify();
    EXPECT_FALSE(verified);
}

TEST(TestRandomBuffer, fill_and_verify_5)
{
    test_random_buffer(5);
}

TEST(TestRandomBuffer, fill_and_verify_77)
{
    test_random_buffer(77);
}

TEST(TestRandomBuffer, fill_and_verify_1041)
{
    test_random_buffer(1041);
}

TEST(TestRandomBuffer, fill_and_verify_1234567)
{
    test_random_buffer(1234567);
}
