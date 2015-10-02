// Copyright (c) 2012 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <sak/error.hpp>

#include <gtest/gtest.h>

namespace
{
    struct dummy_class
    {
        // Test method
        // @throws std::system_error Thrown on failure.
        void test()
        {
            std::error_code ec;
            test(ec);

            // If an error occurs, throw that
            if (ec)
            {
                sak::error::throw_error(ec);
            }
        }

        // Test method
        // @param ec on error set to indicate the type of error
        void test(std::error_code& ec)
        {
            ec = sak::error::failed_open_file;
        }
    };
}


/// Tests error handling with exception
TEST(TestError, ThrowException)
{
    dummy_class dummy;
    std::error_code ec;

    // No exception should be thrown for an empty error
    sak::error::throw_error(ec);

    try
    {
        dummy.test();
    }
    catch (const std::system_error& error)
    {
        ec = error.code();
    }

    EXPECT_EQ(ec, sak::error::failed_open_file);
}

/// Tests error handling with error code
TEST(TestError, ReturnErrorCode)
{
    dummy_class dummy;

    std::error_code ec;
    dummy.test(ec);

    EXPECT_EQ(ec, sak::error::failed_open_file);
}
