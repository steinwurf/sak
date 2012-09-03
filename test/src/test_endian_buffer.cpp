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
#include <sak/endian_buffer.hpp>

#include <cstdlib>

TEST(EndianBuffer, create_buffer)
{
    const uint32_t size = 1024;
    std::vector<uint8_t> buffer;
    buffer.resize(size);

    sak::endian_buffer endian_buffer(&buffer.front(), size);
}

/// Test helper functions

template<class ValueType>
void write_read_test()
{
    const uint32_t elements = 1024;               ///no. of elements
    const uint32_t size = 1024*sizeof(ValueType); ///size in bytes
    std::vector<uint8_t> buffer;
    buffer.resize(size);
    sak::endian_buffer endian_buffer(&buffer.front(), size);

    uint8_t lowest_value = 0;
    uint8_t highest_value = std::numeric_limits<ValueType>::max();

    for(uint32_t i = 0; i < elements; i++)
    {
        endian_buffer.write<ValueType>(highest_value);
    }

    for(uint32_t i = 0; i < elements; i++)
    {
        EXPECT_EQ(highest_value, endian_buffer.read<ValueType>());
    }

    for(uint32_t i = 0; i < elements; i++)
    {
        endian_buffer.write<ValueType>(lowest_value);
    }

    for(uint32_t i = 0; i < elements; i++)
    {
        EXPECT_EQ(lowest_value, endian_buffer.read<ValueType>());
    }
}

template<class ValueType>
void random_write_read_test(bool pseudorandom)
{
    const uint32_t elements = 1024;               /// no. of elements
    const uint32_t size = 1024*sizeof(ValueType); /// size in bytes
    std::vector<uint8_t> buffer;
    buffer.resize(size);
    sak::endian_buffer endian_buffer(&buffer.front(), size);

    uint8_t highest_value = std::numeric_limits<ValueType>::max();

    std::vector<ValueType> values;
    values.resize(elements);

    if(pseudorandom)
    {
        srand(1337);
    }
    else
    {
        srand(static_cast<uint32_t>(time(0)));
    }

    for(uint32_t i = 0; i < elements; i++)
    {
        values[i] = rand() % (highest_value+1);
        endian_buffer.write<ValueType>(values[i]);
    }

    for(int32_t i = elements-1; i >= 0; --i)
    {
        EXPECT_EQ(values[i], endian_buffer.read<ValueType>());
    }
}

void various_write_read_test(bool pseudorandom)
{
    const uint32_t elements = 1024;
    const uint32_t size = 1024*sizeof(uint64_t);
    std::vector<uint8_t> buffer;
    buffer.resize(size);

    sak::endian_buffer endian_buffer(&buffer.front(), size);

    std::vector<uint64_t> values;
    values.resize(elements);

    if(pseudorandom)
    {
        srand(1337);
    }
    else
    {
        srand(static_cast<uint32_t>(time(0)));
    }

    for(uint32_t i = 0; i < elements; i++)
    {
        switch(i % 4)
        {
            case 0:
                values[i] = rand() % std::numeric_limits<uint8_t>::max();
                endian_buffer.write<uint8_t>(values[i]);
                break;
            case 1:
                values[i] = rand() % std::numeric_limits<uint16_t>::max();
                endian_buffer.write<uint16_t>(values[i]);
                break;
            case 2:
                values[i] = rand() % std::numeric_limits<uint32_t>::max();
                endian_buffer.write<uint32_t>(values[i]);
                break;
            case 3:
                values[i] = rand() % std::numeric_limits<uint64_t>::max();
                endian_buffer.write<uint64_t>(values[i]);
                break;
        }
    }

    for(uint32_t i = elements-1; i > 0; i--)
    {
        switch(i % 4)
        {
            case 0:
                EXPECT_EQ(values[i], endian_buffer.read<uint8_t>());
                break;
            case 1:
                EXPECT_EQ(values[i], endian_buffer.read<uint16_t>());
                break;
            case 2:
                EXPECT_EQ(values[i], endian_buffer.read<uint32_t>());
                break;
            case 3:
                EXPECT_EQ(values[i], endian_buffer.read<uint64_t>());
                break;
        }
    }
}

/// Write read tests

TEST(EndianBuffer, read_write_u8)
{
    write_read_test<uint8_t>();
}

TEST(EndianBuffer, read_write_u16)
{
    write_read_test<uint16_t>();
}

TEST(EndianBuffer, read_write_u32)
{
    write_read_test<uint32_t>();
}

TEST(EndianBuffer, read_write_u64)
{
    write_read_test<uint64_t>();
}

/// Pseudorandom write read tests

TEST(EndianBuffer, pseudorandom_read_write_u8)
{
    random_write_read_test<uint8_t>(true);
}

TEST(EndianBuffer, pseudorandom_read_write_u16)
{
    random_write_read_test<uint16_t>(true);
}

TEST(EndianBuffer, pseudorandom_read_write_u32)
{
    random_write_read_test<uint32_t>(true);
}

TEST(EndianBuffer, pseudorandom_read_write_u64)
{
    random_write_read_test<uint64_t>(true);
}

/// Random write read tests

TEST(EndianBuffer, random_read_write_u8)
{
    random_write_read_test<uint8_t>(false);
}

TEST(EndianBuffer, random_read_write_u16)
{
    random_write_read_test<uint16_t>(false);
}

TEST(EndianBuffer, random_read_write_u32)
{
    random_write_read_test<uint32_t>(false);
}

TEST(EndianBuffer, random_read_write_u64)
{
    random_write_read_test<uint64_t>(false);
}

/// Various read writes

TEST(EndianBuffer, pseudorandom_various_read_write)
{
    various_write_read_test(true);
}

TEST(EndianBuffer, random_various_read_write)
{
    various_write_read_test(false);
}
