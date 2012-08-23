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
// DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include <gtest/gtest.h>
#include <sak/endian_buffer.h>


TEST(EndianBuffer, create_buffer)
{
    const unsigned int size = 1024;
    unsigned char* buffer = new unsigned char[size];

    sak::endian_buffer endian_buffer(buffer, size);

    delete buffer;
}

TEST(EndianBuffer, read_write_u8)
{
    const unsigned int size = 1024;
    unsigned char* buffer = new unsigned char[size];

    sak::endian_buffer endian_buffer(buffer, size);

    // Highest possible value
    uint8_t value = 255;

    for(uint32_t i = 0; i < size; i++) {
        endian_buffer.write_u8(value);
    }

    for(uint32_t i = 0; i < size; i++) {
        EXPECT_EQ(endian_buffer.read_u8(), value);
    }

    // Lowest possible value
    value = 0;

    for(uint32_t i = 0; i < size; i++) {
        endian_buffer.write_u8(value);
    }

    for(uint32_t i = 0; i < size; i++) {
        EXPECT_EQ(endian_buffer.read_u8(), value);
    }

    delete buffer;
}

