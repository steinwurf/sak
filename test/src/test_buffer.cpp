// Copyright (c) 2012 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <sak/buffer.hpp>

#include <algorithm>

#include <gtest/gtest.h>

TEST(TestBuffer, construct)
{
    sak::buffer b;
    EXPECT_EQ(0U, b.size());
}

TEST(TestBuffer, resize)
{
    sak::buffer b;
    b.resize(100);
    EXPECT_EQ(100U, b.size());
}

TEST(TestBuffer, resize_and_copy)
{
    sak::buffer b1;
    b1.resize(100);
    EXPECT_EQ(100U, b1.size());

    sak::buffer b2 = b1;
    EXPECT_EQ(100U, b2.size());
}

TEST(TestBuffer, append_to_empty_with_size)
{
    std::vector<uint8_t> data(32);
    EXPECT_EQ(32U, data.size());

    sak::buffer b;
    EXPECT_EQ(0U, b.size());

    b.append(&data[0], static_cast<uint32_t>(data.size()));
    EXPECT_EQ(data.size(), b.size());
}

TEST(TestBuffer, append_to_empty_with_pointers)
{
    std::vector<uint8_t> data(32);
    EXPECT_EQ(32U, data.size());

    sak::buffer b;
    EXPECT_EQ(0U, b.size());

    b.append(&data[0], &data[0] + data.size());
    EXPECT_EQ(data.size(), b.size());
}

TEST(TestBuffer, append_to_empty_with_storage)
{
    std::vector<uint8_t> data(32);
    EXPECT_EQ(32U, data.size());

    sak::buffer b;
    EXPECT_EQ(0U, b.size());

    b.append(sak::storage(data));
    EXPECT_EQ(data.size(), b.size());
}


TEST(TestBuffer, append_to_initialized)
{
    {
        std::vector<uint8_t> data(32, 'x');
        EXPECT_EQ(32U, data.size());

        sak::buffer b(10);
        EXPECT_EQ(0U, b.size());

        b.append(&data[0], static_cast<uint32_t>(data.size()));
        EXPECT_EQ(data.size(), b.size());
        EXPECT_TRUE(std::equal(data.begin(), data.end(), b.data()));
    }

    {
        std::vector<uint8_t> data(32, 'x');
        EXPECT_EQ(32U, data.size());

        sak::buffer b(10);
        EXPECT_EQ(0U, b.size());

        b.append(&data[0], &data[0] + data.size());
        EXPECT_EQ(data.size(), b.size());
        EXPECT_TRUE(std::equal(data.begin(), data.end(), b.data()));
    }

    {
        std::vector<uint8_t> data(32, 'x');
        EXPECT_EQ(32U, data.size());

        sak::buffer b(10);
        EXPECT_EQ(0U, b.size());

        b.append(sak::storage(data));
        EXPECT_EQ(data.size(), b.size());
        EXPECT_TRUE(std::equal(data.begin(), data.end(), b.data()));
    }
}

TEST(TestBuffer, resize_and_clear)
{
    sak::buffer b(100);
    EXPECT_EQ(0U, b.size());
    std::fill_n(b.data(), b.size(), 'x');

    b.resize(10);
    EXPECT_EQ(10U, b.size());
    std::fill_n(b.data(), b.size(), 'x');

    b.resize(101);
    EXPECT_EQ(101U, b.size());
    std::fill_n(b.data(), b.size(), 'x');

    b.clear();
    EXPECT_EQ(0U, b.size());

    b.resize(0);
    EXPECT_EQ(0U, b.size());

    b.resize(102);
    EXPECT_EQ(102U, b.size());
    std::fill_n(b.data(), b.size(), 'x');

    b.resize(0);
    EXPECT_EQ(0U, b.size());

    b.clear();
    EXPECT_EQ(0U, b.size());
}
