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

#include <cstdint>
#include <gtest/gtest.h>
#include <sak/resource_pool.hpp>

struct dummy_object
{
    dummy_object()
    {
        ++m_count;
    }

    ~dummy_object()
    {
        --m_count;
    }


    static int32_t m_count;
};

typedef boost::shared_ptr<dummy_object> dummy_ptr;

int32_t dummy_object::m_count = 0;

boost::shared_ptr<dummy_object> make_dummy()
{
    boost::shared_ptr<dummy_object> obj(new dummy_object);
    return obj;
}


TEST(TestResourcePool, Construct)
{

    {
        sak::resource_pool<dummy_object> pool( boost::bind(make_dummy) );

        EXPECT_EQ(pool.size(), 0U);
        EXPECT_EQ(pool.free(), 0U);

        {
            dummy_ptr d1 = pool.allocate();

            EXPECT_EQ(pool.size(), 1U);
            EXPECT_EQ(pool.free(), 0U);
        }

        EXPECT_EQ(pool.size(), 1U);
        EXPECT_EQ(pool.free(), 1U);

        dummy_ptr d2 = pool.allocate();

        EXPECT_EQ(pool.size(), 1U);
        EXPECT_EQ(pool.free(), 0U);

        dummy_ptr d3 = pool.allocate();

        EXPECT_EQ(pool.size(), 2U);
        EXPECT_EQ(pool.free(), 0U);

        EXPECT_EQ(dummy_object::m_count, 2);

        {
            dummy_ptr d4 = pool.allocate();

            EXPECT_EQ(pool.size(), 3U);
            EXPECT_EQ(pool.free(), 0U);
        }

        EXPECT_EQ(pool.size(), 3U);
        EXPECT_EQ(pool.free(), 1U);

    }

    EXPECT_EQ(dummy_object::m_count, 0);

}


TEST(TestResourcePool, PoolDieBeforeObject)
{

    {
        dummy_ptr d1;
        dummy_ptr d2;
        dummy_ptr d3;

        {
            sak::resource_pool<dummy_object> pool( boost::bind(make_dummy) );

            d1 = pool.allocate();
            d2 = pool.allocate();
            d3 = pool.allocate();

            ASSERT_TRUE(pool.size() == 3);
        }

    }
    ASSERT_TRUE(dummy_object::m_count == 0);

}
