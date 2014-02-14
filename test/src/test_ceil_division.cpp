// Copyright (c) 2012 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <gtest/gtest.h>

#include <sak/ceil_division.hpp>

TEST(TestCeilDivision, ceil_division)
{
    EXPECT_EQ(sak::ceil_division(0, 1), 0);
    EXPECT_EQ(sak::ceil_division(1, 1), 1);
    EXPECT_EQ(sak::ceil_division(1, 2), 1);
    EXPECT_EQ(sak::ceil_division(2, 1), 2);
    EXPECT_EQ(sak::ceil_division(10, 3), 4);
    EXPECT_EQ(sak::ceil_division(3, 10), 1);
}
