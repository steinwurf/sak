// Copyright (c) 2012 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <sak/ceil_division.hpp>

#include <gtest/gtest.h>

TEST(TestCeilDivision, ceil_division)
{
    EXPECT_EQ(sak::ceil_division(0, 1), 0U);
    EXPECT_EQ(sak::ceil_division(1, 1), 1U);
    EXPECT_EQ(sak::ceil_division(1, 2), 1U);
    EXPECT_EQ(sak::ceil_division(2, 1), 2U);
    EXPECT_EQ(sak::ceil_division(10, 3), 4U);
    EXPECT_EQ(sak::ceil_division(3, 10), 1U);
}
