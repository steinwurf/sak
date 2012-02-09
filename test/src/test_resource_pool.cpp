// Copyright (c) 2011, Steinwurf ApS
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met: 
//
// 1. Redistributions of source code must retain the above copyright notice, this
//    list of conditions and the following disclaimer. 
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution. 
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
// ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// The views and conclusions contained in the software and documentation are those
// of the authors and should not be interpreted as representing official policies, 
// either expressed or implied, of the FreeBSD Project.

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




