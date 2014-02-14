// Copyright (c) 2012 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <gtest/gtest.h>
#include <sak/duplex_buffer.hpp>

TEST(TestDuplexBuffer, construct)
{
    sak::duplex_buffer buffer;
    EXPECT_EQ(buffer.size(), 0U);

    buffer = sak::duplex_buffer(100);
    EXPECT_EQ(buffer.size(), 100U);

    buffer = sak::duplex_buffer(10, 100, 100);

    EXPECT_EQ(buffer.size(), 10U);

    sak::duplex_buffer buffer2(buffer);
    EXPECT_EQ(buffer2.size(), 10U);
}


TEST(TestDuplexBuffer, resize_and_fill)
{
    sak::duplex_buffer buffer(10, 100, 100);
    std::fill_n(buffer.data(), buffer.size(), 'b');
    EXPECT_EQ(buffer.size(), 10U);

    buffer.resize_front(100);
    std::fill_n(buffer.data(), buffer.size(), 'b');
    EXPECT_EQ(buffer.size(), 100U);

    buffer.resize_back(100);
    std::fill_n(buffer.data(), buffer.size(), 'b');
    EXPECT_EQ(buffer.size(), 100U);

    buffer.resize_front(10);
    std::fill_n(buffer.data(), buffer.size(), 'b');
    EXPECT_EQ(buffer.size(), 10U);

    buffer.resize_back(100);
    std::fill_n(buffer.data(), buffer.size(), 'b');
    EXPECT_EQ(buffer.size(), 100U);

    buffer.resize_back(200);
    std::fill_n(buffer.data(), buffer.size(), 'b');
    EXPECT_EQ(buffer.size(), 200U);

    buffer.resize_front(10);
    std::fill_n(buffer.data(), buffer.size(), 'b');
    EXPECT_EQ(buffer.size(), 10U);
}
