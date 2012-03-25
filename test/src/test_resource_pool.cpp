// Copyright Steinwurf APS 2011-2012.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <stdint.h>
#include <gtest/gtest.h>
#include <sak/resource_pool.h>

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

	ASSERT_TRUE(pool.size() == 0);
	ASSERT_TRUE(pool.free() == 0);

	{
	    dummy_ptr d1 = pool.allocate();

	    ASSERT_TRUE(pool.size() == 1);
	    ASSERT_TRUE(pool.free() == 0);
	}

	ASSERT_TRUE(pool.size() == 1);
	ASSERT_TRUE(pool.free() == 1);

	dummy_ptr d2 = pool.allocate();

	ASSERT_TRUE(pool.size() == 1);
	ASSERT_TRUE(pool.free() == 0);

	dummy_ptr d3 = pool.allocate();

	ASSERT_TRUE(pool.size() == 2);
	ASSERT_TRUE(pool.free() == 0);

	ASSERT_TRUE(dummy_object::m_count == 2);

	{
	    dummy_ptr d4 = pool.allocate();

	    ASSERT_TRUE(pool.size() == 3);
	    ASSERT_TRUE(pool.free() == 0);
	}

	ASSERT_TRUE(pool.size() == 3);
	ASSERT_TRUE(pool.free() == 1);

    }

    ASSERT_TRUE(dummy_object::m_count == 0);

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




