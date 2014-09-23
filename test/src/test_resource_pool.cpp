// Copyright (c) 2012 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <cstdint>
#include <gtest/gtest.h>
#include <sak/resource_pool.hpp>
#include <sak/is_regular.hpp>

// Put tests classes in an anonymous namespace to avoid violations of
// ODF (one-definition-rule) in other translation units
namespace
{

    // Default constructible dummy object
    struct dummy_one
    {
        dummy_one()
        {
            ++m_count;
        }

        ~dummy_one()
        {
            --m_count;
        }

        // Counter which will check how many object have been allocate
        // and deallocated
        static int32_t m_count;
    };

    int32_t dummy_one::m_count = 0;

    // Non Default constructible dummy object
    struct dummy_two
    {
        dummy_two(uint32_t)
        {
            ++m_count;
        }

        ~dummy_two()
        {
            --m_count;
        }

        static int32_t m_count;
    };

    int32_t dummy_two::m_count = 0;

    std::shared_ptr<dummy_two> make_dummy_two()
    {
        return std::make_shared<dummy_two>(3U);
    }
}

// typedef std::shared_ptr<dummy_object> dummy_ptr;




// std::shared_ptr<dummy_object> make_dummy()
// {
//     std::shared_ptr<dummy_object> obj(new dummy_object);
//     return obj;
// }

// TEST(TestResourcePool, RegularType)
// {
//     EXPECT_TRUE(sak::is_regular<sak::resource_pool<dummy_object>>::value);
// }


// TEST(TestResourcePool, Construct)
// {

//     {
//         sak::resource_pool<dummy_object> pool;
//         pool.set_allocator( std::bind(make_dummy) );

//         EXPECT_EQ(pool.total_resources(), 0U);
//         EXPECT_EQ(pool.unused_resources(), 0U);

//         {
//             dummy_ptr d1 = pool.allocate();

//             EXPECT_EQ(pool.total_resources(), 1U);
//             EXPECT_EQ(pool.unused_resources(), 0U);
//         }

//         EXPECT_EQ(pool.total_resources(), 1U);
//         EXPECT_EQ(pool.unused_resources(), 1U);

//         dummy_ptr d2 = pool.allocate();

//         EXPECT_EQ(pool.total_resources(), 1U);
//         EXPECT_EQ(pool.unused_resources(), 0U);

//         dummy_ptr d3 = pool.allocate();

//         EXPECT_EQ(pool.total_resources(), 2U);
//         EXPECT_EQ(pool.unused_resources(), 0U);

//         EXPECT_EQ(dummy_object::m_count, 2);

//         {
//             dummy_ptr d4 = pool.allocate();

//             EXPECT_EQ(pool.total_resources(), 3U);
//             EXPECT_EQ(pool.unused_resources(), 0U);
//         }

//         EXPECT_EQ(pool.total_resources(), 3U);
//         EXPECT_EQ(pool.unused_resources(), 1U);

//     }

//     EXPECT_EQ(dummy_object::m_count, 0);

// }

/// Test that the pool works for non default constructable objects, if
/// we provide the allocator
TEST(TestResourcePool, NonDefaultConstructable)
{
    {
        sak::resource_pool<dummy_two> pool(make_dummy_two);

        auto o1 = pool.allocate();
        auto o2 = pool.allocate();

        EXPECT_EQ(dummy_two::m_count, 2U);
    }

    EXPECT_EQ(dummy_two::m_count, 0U);

    {
        auto make = []()->std::shared_ptr<dummy_two>
        {
            return std::make_shared<dummy_two>(3U);
        };

        sak::resource_pool<dummy_two> pool(make);

        auto o1 = pool.allocate();
        auto o2 = pool.allocate();

        EXPECT_EQ(dummy_two::m_count, 2U);
    }
}

/// Test that the pool works for non constructable objects, even if
/// we do not provide the allocator
TEST(TestResourcePool, DefaultConstructable)
{
    {
        sak::resource_pool<dummy_one> pool;

        auto o1 = pool.allocate();
        auto o2 = pool.allocate();

        EXPECT_EQ(dummy_one::m_count, 2U);
    }

    EXPECT_EQ(dummy_one::m_count, 0U);
}

/// Test that everything works even if the pool dies before the
// objects allocated
TEST(TestResourcePool, PoolDieBeforeObject)
{
    {
        std::shared_ptr<dummy_one> d1;
        std::shared_ptr<dummy_one> d2;
        std::shared_ptr<dummy_one> d3;

        {
            sak::resource_pool<dummy_one> pool;

            d1 = pool.allocate();
            d2 = pool.allocate();
            d3 = pool.allocate();

            EXPECT_EQ(pool.total_resources(), 3U);
            EXPECT_EQ(dummy_one::m_count, 3U);
        }

        EXPECT_EQ(dummy_one::m_count, 3U);

    }

    EXPECT_EQ(dummy_one::m_count, 0U);
}

/// Test that copying the resource_pool works as expected
///
/// When we copy a resource pool, we will shallow copy it so:
///
///     sak::resource_pool<dummy_one> pool;
///
///     auto o1 = pool.allocate();
///     auto o2 = pool.allocate();
///
///     sak::resource_pool<dummy_one> new_pool = pool;
///
///     o1.reset();
///
///
///
///

TEST(TestResourcePool, Copy)
{
    // {
    //     sak::resource_pool<dummy_one> pool;

    //     auto o1 = pool.allocate();
    //     auto o2 = pool.allocate();

    //     EXPECT_EQ(dummy_one::m_count, 2U);
    // }

    // EXPECT_EQ(dummy_one::m_count, 0U);
}
