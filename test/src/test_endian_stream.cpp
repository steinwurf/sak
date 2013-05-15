// Copyright (c) 2012-2013, Steinwurf ApS
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

#include <cstdlib>
#include <vector>
#include <string>
#include <algorithm>

#include <gtest/gtest.h>

#include <sak/endian_stream.hpp>

TEST(TestEndianStream, create_stream)
{
    const uint32_t size = 1024;
    std::vector<uint8_t> buffer;
    buffer.resize(size);

    sak::endian_stream stream(buffer.data(), size);
}

TEST(TestEndianStream, create_stream_from_storage)
{
    const uint32_t elements = 10;               ///no. of elements
    const uint32_t size = elements * sizeof(uint32_t);

    std::vector<uint8_t> buffer;
    buffer.resize(size);
    // Create endian stream directly from sak::storage
    sak::endian_stream stream(sak::storage(buffer));

    for (uint32_t i = 0; i < elements; i++)
    {
        stream.write(i);
    }

    // Go back to the beginning of the stream
    stream.seek(0);
    uint32_t last_value = 0;
    for (uint32_t i = 0; i < elements; i++)
    {
        stream.read(last_value);
        EXPECT_EQ(i, last_value);
    }
}

/// Test helper functions

template<class ValueType>
void write_read_test()
{
    const uint32_t elements = 1024;               ///no. of elements
    const uint32_t size = 1024 * sizeof(ValueType); ///size in bytes
    std::vector<uint8_t> buffer;
    buffer.resize(size);
    sak::endian_stream stream(buffer.data(), size);

    ValueType lowest_value = 0;
    ValueType highest_value = std::numeric_limits<ValueType>::max();
    ValueType last_value = 0;

    for (uint32_t i = 0; i < elements; i++)
    {
        stream.write(highest_value);
    }

    // Go back to the beginning of the stream
    stream.seek(0);
    for (uint32_t i = 0; i < elements; i++)
    {
        stream.read(last_value);
        EXPECT_EQ(highest_value, last_value);
    }

    stream.seek(0);
    for (uint32_t i = 0; i < elements; i++)
    {
        stream.write(lowest_value);
    }

    stream.seek(0);
    for (uint32_t i = 0; i < elements; i++)
    {
        stream.read(last_value);
        EXPECT_EQ(lowest_value, last_value);
    }
}

template<class ValueType>
void random_write_read_test(bool pseudorandom)
{
    const uint32_t elements = 1024;               /// no. of elements
    const uint32_t size = 1024 * sizeof(ValueType); /// size in bytes
    std::vector<uint8_t> buffer;
    buffer.resize(size);

    sak::endian_stream stream(buffer.data(), size);

    ValueType highest_value = std::numeric_limits<ValueType>::max();

    std::vector<ValueType> values;
    values.resize(elements);

    if (pseudorandom)
    {
        srand(1337);
    }
    else
    {
        srand(static_cast<uint32_t>(time(0)));
    }

    for (uint32_t i = 0; i < elements; i++)
    {
        values[i] = rand() % (highest_value);
        stream.write(values[i]);
    }

    ValueType last_value = 0;
    // Go back to the beginning of the stream
    stream.seek(0);
    // Read values in FIFO order
    for (uint32_t i = 0; i < elements; i++)
    {
        stream.read(last_value);
        EXPECT_EQ(values[i], last_value);
    }
}

void various_write_read_test(bool pseudorandom)
{
    const uint32_t elements = 1024;
    const uint32_t size = 1024 * sizeof(uint64_t);
    std::vector<uint8_t> buffer;
    buffer.resize(size);

    sak::endian_stream stream(buffer.data(), size);

    std::vector<uint64_t> values;
    values.resize(elements);

    if (pseudorandom)
    {
        srand(1337);
    }
    else
    {
        srand(static_cast<uint32_t>(time(0)));
    }

    for (uint32_t i = 0; i < elements; i++)
    {
        switch (i % 4)
        {
            case 0:
                values[i] = rand() % std::numeric_limits<uint8_t>::max();
                stream.write((uint8_t)values[i]);
                break;
            case 1:
                values[i] = rand() % std::numeric_limits<uint16_t>::max();
                stream.write((uint16_t)values[i]);
                break;
            case 2:
                values[i] = rand() % std::numeric_limits<uint32_t>::max();
                stream.write((uint32_t)values[i]);
                break;
            case 3:
                values[i] = rand() % std::numeric_limits<uint64_t>::max();
                stream.write((uint64_t)values[i]);
                break;
        }
    }

    uint8_t last_u8 = 0;
    uint16_t last_u16 = 0;
    uint32_t last_u32 = 0;
    uint64_t last_u64 = 0;
    // Go back to the beginning of the stream
    stream.seek(0);
    // Read values in FIFO order
    for (uint32_t i = 0; i < elements; i++)
    {
        switch (i % 4)
        {
            case 0:
                stream.read<uint8_t>(last_u8);
                EXPECT_EQ(values[i], last_u8);
                break;
            case 1:
                stream.read<uint16_t>(last_u16);
                EXPECT_EQ(values[i], last_u16);
                break;
            case 2:
                stream.read<uint32_t>(last_u32);
                EXPECT_EQ(values[i], last_u32);
                break;
            case 3:
                stream.read<uint64_t>(last_u64);
                EXPECT_EQ(values[i], last_u64);
                break;
        }
    }
}

/// Write-read tests

TEST(TestEndianStream, read_write_u8)
{
    write_read_test<uint8_t>();
}

TEST(TestEndianStream, read_write_u16)
{
    write_read_test<uint16_t>();
}

TEST(TestEndianStream, read_write_u32)
{
    write_read_test<uint32_t>();
}

TEST(TestEndianStream, read_write_u64)
{
    write_read_test<uint64_t>();
}

/// Pseudorandom write-read tests

TEST(TestEndianStream, pseudorandom_read_write_u8)
{
    random_write_read_test<uint8_t>(true);
}

TEST(TestEndianStream, pseudorandom_read_write_u16)
{
    random_write_read_test<uint16_t>(true);
}

TEST(TestEndianStream, pseudorandom_read_write_u32)
{
    random_write_read_test<uint32_t>(true);
}

TEST(TestEndianStream, pseudorandom_read_write_u64)
{
    random_write_read_test<uint64_t>(true);
}

/// Random write read tests

TEST(TestEndianStream, random_read_write_u8)
{
    random_write_read_test<uint8_t>(false);
}

TEST(TestEndianStream, random_read_write_u16)
{
    random_write_read_test<uint16_t>(false);
}

TEST(TestEndianStream, random_read_write_u32)
{
    random_write_read_test<uint32_t>(false);
}

TEST(TestEndianStream, random_read_write_u64)
{
    random_write_read_test<uint64_t>(false);
}

/// Various read writes

TEST(TestEndianStream, pseudorandom_various_read_write)
{
    various_write_read_test(true);
}

TEST(TestEndianStream, random_various_read_write)
{
    various_write_read_test(false);
}

/// Test composite data types

TEST(TestEndianStream, read_write_string)
{
    const uint32_t size = 1024;
    std::vector<uint8_t> buffer;
    buffer.resize(size);

    sak::endian_stream stream(buffer.data(), size);

    std::string first("first first first");
    std::string second("second second");
    std::string third("third");

    // Write the strings together with their lengths
    // The length is written as 16-bit integers
    stream.write((uint16_t)first.size());
    stream.write(sak::storage(first));
    stream.write((uint16_t)second.size());
    stream.write(sak::storage(second));
    stream.write((uint16_t)third.size());
    stream.write(sak::storage(third));

    // Temp variables
    std::string current;
    uint16_t len = 0;

    // Go back to the beginning of the stream
    stream.seek(0);

    // Read the strings together with their lengths
    stream.read(len);
    EXPECT_EQ(first.size(), len);
    // Resize the current string to accommodate 'len' bytes
    current.resize(len);
    stream.read(sak::storage(current));
    EXPECT_EQ(first, current);

    stream.read(len);
    EXPECT_EQ(second.size(), len);
    current.resize(len);
    stream.read(sak::storage(current));
    EXPECT_EQ(second, current);

    stream.read(len);
    EXPECT_EQ(third.size(), len);
    current.resize(len);
    stream.read(sak::storage(current));
    EXPECT_EQ(third, current);
}

TEST(TestEndianStream, read_write_vector)
{
    const uint32_t size = 1024;
    std::vector<uint8_t> buffer;
    buffer.resize(size);
    sak::endian_stream stream(buffer.data(), size);

    std::vector<uint8_t> first(100, 'a');
    std::vector<uint32_t> second(200, 1234);

    // Write the vectors together with their lengths
    // The length is written as 16-bit integers
    stream.write((uint16_t)first.size());
    stream.write(sak::storage(first));
    // The size here refers to the number of integers
    // stored in the second vector
    stream.write((uint16_t)second.size());
    stream.write(sak::storage(second));

    // Temp variables
    std::vector<uint8_t> first_out;
    std::vector<uint32_t> second_out;
    uint16_t len = 0;

    // Go back to the beginning of the stream
    stream.seek(0);

    // Read the vector length
    stream.read(len);
    EXPECT_EQ(first.size(), len);
    // Resize the output vector to accommodate 'len' bytes
    first_out.resize(len);
    stream.read(sak::storage(first_out));
    EXPECT_TRUE(
        std::equal(first.begin(), first.end(), first_out.begin()));

    // Read the vector length
    stream.read(len);
    EXPECT_EQ(second.size(), len);
    // Resize the output vector to accommodate 'len' bytes
    second_out.resize(len);
    stream.read(sak::storage(second_out));
    EXPECT_TRUE(
        std::equal(second.begin(), second.end(), second_out.begin()));
}
