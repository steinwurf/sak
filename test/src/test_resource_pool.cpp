// Copyright (c) 2012 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

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
