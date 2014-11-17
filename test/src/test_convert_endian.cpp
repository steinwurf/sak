// Copyright (c) 2012 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <sak/convert_endian.hpp>

#include <gtest/gtest.h>

// From test proposed here:
// http://stackoverflow.com/questions/1001307/
bool is_big_endian()
{
    union
    {
        uint32_t i;
        uint8_t c[4];
    } bint = {0x01020304};

    return bint.c[0] == 1;
}

TEST(ConvertEndian, CheckEndian)
{
    EXPECT_TRUE(is_big_endian() == sak::host_endian::big_endian);
}

TEST(ConvertEndian, Convert)
{
    // Indicated endianness for debugging purposes
    SCOPED_TRACE(
        testing::Message() << "big_endian:" << sak::host_endian::big_endian);

    // Test 8-bit integer
    {
        uint8_t data[1];
        uint8_t in = 0x11U;

        // No change will occur for a single-byte value
        sak::big_endian::put8(in, data);
        EXPECT_TRUE(0x11U == data[0]);

        // No change will occur for a single-byte value
        uint8_t out = sak::big_endian::get8(data);
        EXPECT_TRUE(out == in);
    }

    // Test 16-bit integer
    {
        uint8_t data[2];
        uint16_t in = 0x1122U;

        // If the host is little endian, the put function
        // should change the byte order (no change for big endian)
        sak::big_endian::put16(in, data);
        EXPECT_TRUE(0x11U == data[0]);
        EXPECT_TRUE(0x22U == data[1]);

        // Get should swap the value back (no change for big endian)
        uint16_t out = sak::big_endian::get16(data);
        EXPECT_TRUE(out == in);
    }

    // Test 32-bit integer
    {
        uint8_t data[4];
        uint32_t in = 0x11223344U;

        // If the host is little endian, the put function
        // should change the byte order (no change for big endian)
        sak::big_endian::put32(in, data);
        EXPECT_TRUE(0x11U == data[0]);
        EXPECT_TRUE(0x22U == data[1]);
        EXPECT_TRUE(0x33U == data[2]);
        EXPECT_TRUE(0x44U == data[3]);

        // Get should swap the value back (no change for big endian)
        uint32_t out = sak::big_endian::get32(data);
        EXPECT_TRUE(out == in);
    }

    // Test 64-bit integer
    {
        uint8_t data[8];
        uint64_t in = 0x1122334455667788ULL;

        // If the host is little endian, the put function
        // should change the byte order (no change for big endian)
        sak::big_endian::put64(in, data);
        EXPECT_TRUE(0x11U == data[0]);
        EXPECT_TRUE(0x22U == data[1]);
        EXPECT_TRUE(0x33U == data[2]);
        EXPECT_TRUE(0x44U == data[3]);
        EXPECT_TRUE(0x55U == data[4]);
        EXPECT_TRUE(0x66U == data[5]);
        EXPECT_TRUE(0x77U == data[6]);
        EXPECT_TRUE(0x88U == data[7]);

        // Get should swap the value back (no change for big endian)
        uint64_t out = sak::big_endian::get64(data);
        EXPECT_TRUE(out == in);
    }
}

TEST(ConvertEndian, ConvertTemplate)
{
    // Indicated endianness for debugging purposes
    SCOPED_TRACE(
        testing::Message() << "big_endian:" << sak::host_endian::big_endian);

    // Test 16-bit integer
    {
        uint8_t data[2];
        uint16_t in = 0x1020U;

        // If the host is little endian, the put function
        // should change the byte order (no change for big endian)
        sak::big_endian::put<uint16_t>(in, data);
        EXPECT_TRUE(0x10U == data[0]);
        EXPECT_TRUE(0x20U == data[1]);

        // Get should swap the value back (no change for big endian)
        uint16_t out = sak::big_endian::get<uint16_t>(data);
        EXPECT_TRUE(out == in);
    }

    // Test 32-bit integer
    {
        uint8_t data[4];
        uint32_t in = 0x10203040U;

        // If the host is little endian, the put function
        // should change the byte order (no change for big endian)
        sak::big_endian::put<uint32_t>(in, data);
        EXPECT_TRUE(0x10U == data[0]);
        EXPECT_TRUE(0x20U == data[1]);
        EXPECT_TRUE(0x30U == data[2]);
        EXPECT_TRUE(0x40U == data[3]);

        // Get should swap the value back
        uint32_t out = sak::big_endian::get<uint32_t>(data);
        EXPECT_TRUE(out == in);
    }

    // Test 64-bit integer
    {
        uint8_t data[8];
        uint64_t in = 0x1020304050607080ULL;

        // If the host is little endian, the put function
        // should change the byte order (no change for big endian)
        sak::big_endian::put<uint64_t>(in, data);
        EXPECT_TRUE(0x10U == data[0]);
        EXPECT_TRUE(0x20U == data[1]);
        EXPECT_TRUE(0x30U == data[2]);
        EXPECT_TRUE(0x40U == data[3]);
        EXPECT_TRUE(0x50U == data[4]);
        EXPECT_TRUE(0x60U == data[5]);
        EXPECT_TRUE(0x70U == data[6]);
        EXPECT_TRUE(0x80U == data[7]);

        // Get should swap the value back (no change for big endian)
        uint64_t out = sak::big_endian::get<uint64_t>(data);
        EXPECT_TRUE(out == in);
    }
}
