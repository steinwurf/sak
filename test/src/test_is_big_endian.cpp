// Copyright (c) 2012 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <sak/is_big_endian.hpp>

#include <gtest/gtest.h>

TEST(IsBigEndian, CheckEndian)
{
    // Alternative endianness check from here:
    // http://esr.ibiblio.org/?p=5095
    bool alternative_check = (*(uint16_t *)"\0\xff" < 0x100);
    EXPECT_TRUE(sak::is_big_endian() == alternative_check);
}
