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
#include <sak/is_aligned.hpp>

TEST(TestIsAligned, is_aligned8)
{
    EXPECT_EQ(sak::is_aligned((void*)0x00000000, 8), true);
    EXPECT_EQ(sak::is_aligned((void*)0x00000001, 8), false);
    EXPECT_EQ(sak::is_aligned((void*)0x00000002, 8), false);
    EXPECT_EQ(sak::is_aligned((void*)0x00000003, 8), false);
    EXPECT_EQ(sak::is_aligned((void*)0x00000004, 8), false);
    EXPECT_EQ(sak::is_aligned((void*)0x00000005, 8), false);
    EXPECT_EQ(sak::is_aligned((void*)0x00000006, 8), false);
    EXPECT_EQ(sak::is_aligned((void*)0x00000007, 8), false);
    EXPECT_EQ(sak::is_aligned((void*)0x00000008, 8), true);
}

TEST(TestIsAligned, is_aligned16)
{
    EXPECT_EQ(sak::is_aligned((void*)0x00000000, 16), true);
    EXPECT_EQ(sak::is_aligned((void*)0x00000001, 16), false);
    EXPECT_EQ(sak::is_aligned((void*)0x00000002, 16), false);
    EXPECT_EQ(sak::is_aligned((void*)0x00000003, 16), false);
    EXPECT_EQ(sak::is_aligned((void*)0x00000004, 16), false);
    EXPECT_EQ(sak::is_aligned((void*)0x00000005, 16), false);
    EXPECT_EQ(sak::is_aligned((void*)0x00000006, 16), false);
    EXPECT_EQ(sak::is_aligned((void*)0x00000007, 16), false);
    EXPECT_EQ(sak::is_aligned((void*)0x00000008, 16), false);
    EXPECT_EQ(sak::is_aligned((void*)0x00000009, 16), false);
    EXPECT_EQ(sak::is_aligned((void*)0x0000000A, 16), false);
    EXPECT_EQ(sak::is_aligned((void*)0x0000000B, 16), false);
    EXPECT_EQ(sak::is_aligned((void*)0x0000000C, 16), false);
    EXPECT_EQ(sak::is_aligned((void*)0x0000000D, 16), false);
    EXPECT_EQ(sak::is_aligned((void*)0x0000000E, 16), false);
    EXPECT_EQ(sak::is_aligned((void*)0x0000000F, 16), false);
    EXPECT_EQ(sak::is_aligned((void*)0x00000010, 16), true);
}
