// Copyright Steinwurf APS 2011-2012.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <gtest/gtest.h>
#include <sak/convert_endian.h>

// From test proposed here:
// http://stackoverflow.com/questions/1001307/
bool is_big_endian()
{
    union {
        uint32_t i;
        char c[4];
    } bint = {0x01020304};

    return bint.c[0] == 1;
}

TEST(ConvertEndian, CheckEndian)
{
    EXPECT_TRUE(is_big_endian() == sak::host_endian::big_endian);
}

TEST(ConvertEndian, Convert)
{

    {
        uint8_t data[1];
        uint8_t in  = 0x11U;

        // If the host is big endian the put function
        // should not change the byte order
        sak::big_endian::put8(in, data);
        EXPECT_TRUE(0x11U == data[0]);

        // Neither should the get
        uint8_t out = sak::big_endian::get8(data);
        EXPECT_TRUE(out == in);

    }

    {
        if(sak::host_endian::big_endian)
        {
            uint8_t data[2];
            uint16_t in  = 0x1122U;

            // If the host is big endian the put function
            // should not change the byte order
            sak::big_endian::put16(in, data);
            EXPECT_TRUE(0x11U == data[0]);
            EXPECT_TRUE(0x22U == data[1]);

            // Neither should the get
            uint16_t out = sak::big_endian::get16(data);
            EXPECT_TRUE(out == in);
        }
        else
        {
            uint8_t data[2];
            uint16_t in  = 0x1122U;

            // If the host is little endian the put function
            // should change the byte order
            sak::big_endian::put16(in, data);
            EXPECT_TRUE(0x11U == data[0]);
            EXPECT_TRUE(0x22U == data[1]);

            // Get should swap the value back
            uint16_t out = sak::big_endian::get16(data);
            EXPECT_TRUE(out == in);
        }

    }

    {
        if(sak::host_endian::big_endian)
        {
            uint8_t data[4];
            uint32_t in  = 0x11223344U;

            // If the host is big endian the put function
            // should not change the byte order
            sak::big_endian::put32(in, data);
            EXPECT_TRUE(0x11U == data[0]);
            EXPECT_TRUE(0x22U == data[1]);
            EXPECT_TRUE(0x33U == data[2]);
            EXPECT_TRUE(0x44U == data[3]);

            // Neither should the get
            uint32_t out = sak::big_endian::get32(data);
            EXPECT_TRUE(out == in);
        }
        else
        {
            uint8_t data[4];
            uint32_t in  = 0x11223344U;

            // If the host is little endian the put function
            // should change the byte order
            sak::big_endian::put32(in, data);
            EXPECT_TRUE(0x11U == data[0]);
            EXPECT_TRUE(0x22U == data[1]);
            EXPECT_TRUE(0x33U == data[2]);
            EXPECT_TRUE(0x44U == data[3]);

            // Get should swap the value back
            uint32_t out = sak::big_endian::get32(data);
            EXPECT_TRUE(out == in);
        }

    }


    {
        if(sak::host_endian::big_endian)
        {
            uint8_t data[8];
            uint64_t in  = 0x1122334455667788ULL;

            // If the host is big endian the put function
            // should not change the byte order
            sak::big_endian::put64(in, reinterpret_cast<uint8_t*>(data));
            EXPECT_TRUE(0x11U == data[0]);
            EXPECT_TRUE(0x22U == data[1]);
            EXPECT_TRUE(0x33U == data[2]);
            EXPECT_TRUE(0x44U == data[3]);
            EXPECT_TRUE(0x55U == data[4]);
            EXPECT_TRUE(0x66U == data[5]);
            EXPECT_TRUE(0x77U == data[6]);
            EXPECT_TRUE(0x88U == data[7]);


            // Neither should the get
            uint64_t out = sak::big_endian::get64(data);
            EXPECT_TRUE(out == in);
        }
        else
        {
            uint8_t data[8];
            uint64_t in  = 0x1122334455667788ULL;

            // If the host is little endian the put function
            // should change the byte order
            sak::big_endian::put64(in, data);
            EXPECT_TRUE(0x11U == data[0]);
            EXPECT_TRUE(0x22U == data[1]);
            EXPECT_TRUE(0x33U == data[2]);
            EXPECT_TRUE(0x44U == data[3]);
            EXPECT_TRUE(0x55U == data[4]);
            EXPECT_TRUE(0x66U == data[5]);
            EXPECT_TRUE(0x77U == data[6]);
            EXPECT_TRUE(0x88U == data[7]);

            // Get should swap the value back
            uint64_t out = sak::big_endian::get64(data);
            EXPECT_TRUE(out == in);
        }

    }

}


TEST(ConvertEndian, ConvertTemplate)
{
    {
        if(sak::host_endian::big_endian)
        {
            uint8_t data[2];
            uint16_t in  = 0x1122U;

            // If the host is big endian the put function
            // should not change the byte order
            sak::big_endian::put<uint16_t>(in, data);
            EXPECT_TRUE(0x11U == data[0]);
            EXPECT_TRUE(0x22U == data[1]);

            // Neither should the get
            uint16_t out = sak::big_endian::get<uint16_t>(data);
            EXPECT_TRUE(out == in);
        }
        else
        {
            uint8_t data[2];
            uint16_t in  = 0x1122U;

            // If the host is little endian the put function
            // should change the byte order
            sak::big_endian::put<uint16_t>(in, data);
            EXPECT_TRUE(0x11U == data[0]);
            EXPECT_TRUE(0x22U == data[1]);

            // Get should swap the value back
            uint16_t out = sak::big_endian::get<uint16_t>(data);
            EXPECT_TRUE(out == in);
        }

    }
}



