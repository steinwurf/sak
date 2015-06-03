// Copyright (c) 2012 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <sak/duplex_buffer.hpp>

#include <gtest/gtest.h>

TEST(TestDuplexBuffer, construct)
{
    sak::duplex_buffer buffer;
    EXPECT_EQ(buffer.size(), 0U);

    EXPECT_EQ(buffer.data(), buffer.front());
    EXPECT_EQ(buffer.data() + buffer.size(), buffer.back());

    buffer = sak::duplex_buffer(100);
    EXPECT_EQ(buffer.size(), 100U);

    EXPECT_EQ(buffer.data(), buffer.front());
    EXPECT_EQ(buffer.data() + buffer.size(), buffer.back());

    buffer = sak::duplex_buffer(10, 100, 100);

    EXPECT_EQ(buffer.size(), 10U);

    const sak::duplex_buffer buffer2(buffer);
    EXPECT_EQ(buffer2.size(), 10U);

    EXPECT_EQ(buffer2.data(), buffer2.front());
    EXPECT_EQ(buffer2.data() + buffer2.size(), buffer2.back());
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
