// Copyright (c) 2012-2014, Steinwurf ApS
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

#include <gtest/gtest.h>

#include <sak/buffer.hpp>

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
    }

    {
        std::vector<uint8_t> data(32, 'x');
        EXPECT_EQ(32U, data.size());

        sak::buffer b(10);
        EXPECT_EQ(0U, b.size());

        b.append(&data[0], &data[0] + data.size());
        EXPECT_EQ(data.size(), b.size());
    }

    {
        std::vector<uint8_t> data(32, 'x');
        EXPECT_EQ(32U, data.size());

        sak::buffer b(10);
        EXPECT_EQ(0U, b.size());

        b.append(sak::storage(data));
        EXPECT_EQ(data.size(), b.size());
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
