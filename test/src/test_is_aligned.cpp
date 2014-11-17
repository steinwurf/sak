// Copyright (c) 2012 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <sak/is_aligned.hpp>

#include <gtest/gtest.h>

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
