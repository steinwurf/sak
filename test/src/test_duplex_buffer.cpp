// Copyright (c) 2012, Steinwurf ApS
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
