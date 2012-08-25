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
    uint8_t* buffer = new uint8_t[size];

    sak::endian_buffer endian_buffer(buffer, size);

    delete buffer;
}

TEST(EndianBuffer, read_write_u8)
{
    const uint32_t size = 1024;
    uint8_t* buffer = new uint8_t[size];

    sak::endian_buffer endian_buffer(buffer, size);

    uint8_t lowest_value = 0;
    uint8_t highest_value = UINT8_MAX;

    for(uint32_t i = 0; i < size; i++) {
        endian_buffer.write_u8(highest_value);
    }

    for(uint32_t i = 0; i < size; i++) {
        EXPECT_EQ(highest_value, endian_buffer.read_u8());
    }

    for(uint32_t i = 0; i < size; i++) {
        endian_buffer.write_u8(lowest_value);
    }

    for(uint32_t i = 0; i < size; i++) {
        EXPECT_EQ(lowest_value, endian_buffer.read_u8());
    }

    std::vector<uint8_t> values(size);
    /* initialize random seed with the hardcoded seed */
    srand(1337);

    for(uint32_t i = 0; i < size; i++) {
        values[i] = rand() % highest_value+1;
        endian_buffer.write_u8(values[i]);
    }

    for(uint32_t i = size-1; i > 0; i--) {
        EXPECT_EQ(values[i], endian_buffer.read_u8());
    }

    delete buffer;
}

TEST(EndianBuffer, read_write_u16)
{
    const uint32_t size = 1024;
    uint8_t* buffer = new uint8_t[size*2]; //We need twice the memory

    sak::endian_buffer endian_buffer(buffer, size);

    uint16_t lowest_value = 0;
    uint16_t highest_value = UINT16_MAX;

    for(uint32_t i = 0; i < size; i++) {
        endian_buffer.write_u16(highest_value);
    }

    for(uint32_t i = 0; i < size; i++) {
        EXPECT_EQ(highest_value, endian_buffer.read_u16());
    }

    for(uint32_t i = 0; i < size; i++) {
        endian_buffer.write_u16(lowest_value);
    }

    for(uint32_t i = 0; i < size; i++) {
        EXPECT_EQ(lowest_value, endian_buffer.read_u16());
    }


    std::vector<uint16_t> values(size);

    /* initialize random seed with the hardcoded seed */
    srand(1337);

    for(uint32_t i = 0; i < size; i++) {
        values[i] = rand() % highest_value+1;
        endian_buffer.write_u16(values[i]);
    }

    for(uint32_t i = size-1; i > 0; i--) {
        EXPECT_EQ(values[i], endian_buffer.read_u16());
    }

    delete buffer;
}


TEST(EndianBuffer, read_write_u32)
{
    const uint32_t size = 1024;
    uint8_t* buffer = new uint8_t[size*4]; //We need 4 times the memory

    sak::endian_buffer endian_buffer(buffer, size);

    uint32_t lowest_value = 0;
    uint32_t highest_value = UINT32_MAX;

    for(uint32_t i = 0; i < size; i++) {
        endian_buffer.write_u32(highest_value);
    }

    for(uint32_t i = 0; i < size; i++) {
        EXPECT_EQ(highest_value, endian_buffer.read_u32());
    }

    for(uint32_t i = 0; i < size; i++) {
        endian_buffer.write_u32(lowest_value);
    }

    for(uint32_t i = 0; i < size; i++) {
        EXPECT_EQ(lowest_value, endian_buffer.read_u32());
    }


    std::vector<uint32_t> values(size);

    /* initialize random seed with the hardcoded seed */
    srand(1337);

    for(uint32_t i = 0; i < size; i++) {
        values[i] = rand() % highest_value+1;
        endian_buffer.write_u32(values[i]);
    }

    for(uint32_t i = size-1; i > 0; i--) {
        EXPECT_EQ(values[i], endian_buffer.read_u32());
    }

    delete buffer;
}


TEST(EndianBuffer, read_write_u64)
{
    const uint32_t size = 1024;
    uint8_t* buffer = new uint8_t[size*8]; //We need 8 times the memory

    sak::endian_buffer endian_buffer(buffer, size);

    uint64_t lowest_value = 0;
    uint64_t highest_value = UINT64_MAX;

    for(uint32_t i = 0; i < size; i++) {
        endian_buffer.write_u64(highest_value);
    }

    for(uint32_t i = 0; i < size; i++) {
        EXPECT_EQ(highest_value, endian_buffer.read_u64());
    }

    for(uint32_t i = 0; i < size; i++) {
        endian_buffer.write_u64(lowest_value);
    }

    for(uint32_t i = 0; i < size; i++) {
        EXPECT_EQ(lowest_value, endian_buffer.read_u64());
    }


    std::vector<uint64_t> values(size);

    /* initialize random seed with the hardcoded seed */
    srand(1337);

    for(uint32_t i = 0; i < size; i++) {
        values[i] = rand() % highest_value+1;
        endian_buffer.write_u64(values[i]);
    }

    for(uint32_t i = size-1; i > 0; i--) {
        EXPECT_EQ(values[i], endian_buffer.read_u64());
    }

    delete buffer;
}


TEST(EndianBuffer, various_read_write)
{
    const uint32_t size = 1024;
    uint8_t* buffer = new uint8_t[size*8]; //We need at most 8 times the memory

    sak::endian_buffer endian_buffer(buffer, size);

    std::vector<uint64_t> values(size);

    /* initialize random seed with the hardcoded seed */
    srand(1337);

    for(uint32_t i = 0; i < size; i++) {
        switch(i % 4) {
            case 0:
                values[i] = rand() % UINT8_MAX;
                endian_buffer.write_u8(values[i]);
                break;
            case 1:
                values[i] = rand() % UINT16_MAX;
                endian_buffer.write_u16(values[i]);
                break;
            case 2:
                values[i] = rand() % UINT32_MAX;
                endian_buffer.write_u32(values[i]);
                break;
            case 3:
                values[i] = rand() % UINT64_MAX;
                endian_buffer.write_u64(values[i]);
                break;
        }
    }

    for(uint32_t i = size-1; i > 0; i--) {
        switch(i % 4) {
            case 0:
                EXPECT_EQ(values[i], endian_buffer.read_u8());
                break;
            case 1:
                EXPECT_EQ(values[i], endian_buffer.read_u16());
                break;
            case 2:
                EXPECT_EQ(values[i], endian_buffer.read_u32());
                break;
            case 3:
                EXPECT_EQ(values[i], endian_buffer.read_u64());
                break;
        }
    }

    delete buffer;
}
