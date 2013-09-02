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
#include <sak/aligned_allocator.hpp>
#include <sak/is_aligned.hpp>

TEST(TestAlignedAllocator, construct)
{
    sak::aligned_allocator<uint8_t> alloc;
    EXPECT_EQ(alloc.alignment(), sak::platform::simd_align);
}


TEST(TestAlignedAllocator, vector)
{
    typedef std::vector<uint32_t, sak::aligned_allocator<uint32_t> >
    aligned_vector;

    aligned_vector v;

    v.push_back(42);
    v.push_back(56);
    v.push_back(11);

    EXPECT_TRUE(sak::is_aligned(&v[0]));

    v.push_back(42);
    v.push_back(56);
    v.push_back(11);

    EXPECT_TRUE(sak::is_aligned(&v[0]));

    v.push_back(42);
    v.push_back(56);
    v.push_back(11);

    EXPECT_TRUE(sak::is_aligned(&v[0]));

    aligned_vector v2 = v;

    EXPECT_TRUE(sak::is_aligned(&v2[0]));

    v2.push_back(42);
    v2.push_back(56);
    v2.push_back(11);

    EXPECT_TRUE(sak::is_aligned(&v2[0]));

    v2.push_back(42);
    v2.push_back(56);
    v2.push_back(11);

    EXPECT_TRUE(sak::is_aligned(&v2[0]));

}

TEST(TestAlignedAllocator, vector_resize)
{
    typedef std::vector<uint8_t, sak::aligned_allocator<uint8_t> >
    aligned_vector;

    aligned_vector v;

    v.resize(100);

    EXPECT_EQ(v.size(), 100U);
    v[10] = 0;
}


TEST(TestAlignedAllocator, vector_zero_size)
{
    typedef std::vector<uint8_t, sak::aligned_allocator<uint8_t> >
    aligned_vector;

    aligned_vector v(0);
    EXPECT_EQ(v.size(), 0U);
}
