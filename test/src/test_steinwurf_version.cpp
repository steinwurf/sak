// Copyright (c) 2012 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <sak/steinwurf_version.hpp>

#include <gtest/gtest.h>

TEST(TestVersion, not_empty)
{
    EXPECT_FALSE(sak::steinwurf_version().empty());
}
