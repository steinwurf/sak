// Copyright (c) 2012 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <cstdint>
#include <gtest/gtest.h>
#include <sak/resource_pool.hpp>

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

        void recycle()
        { }


        ~dummy_one()
        {
            --m_count;
        }

        // Counter which will check how many object have been allocate
        // and deallocated
        static int32_t m_count;
    };

    int32_t dummy_one::m_count = 0;

    std::shared_ptr<dummy_one> make_dummy_one()
    {
        return std::make_shared<dummy_one>();
    }

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

    std::shared_ptr<dummy_two> make_dummy_two(uint32_t v)
    {
        return std::make_shared<dummy_two>(v);
    }
}

/// Test that our resource pool is a regular type. We are not
/// implementing equality or less than here, but maybe we could.
namespace
{
    /// This code checks whether a type is regular or not. See the
    /// Eric Niebler's talk from C++Now
    /// 2014. http://youtu.be/zgOF4NrQllo
    template<class T>
    struct is_regular :
        std::integral_constant<bool,
        std::is_default_constructible<T>::value &&
        std::is_copy_constructible<T>::value &&
        std::is_move_constructible<T>::value &&
        std::is_copy_assignable<T>::value &&
        std::is_move_assignable<T>::value>
    { };
}

TEST(TestResourcePool, RegularType)
{
    EXPECT_TRUE(is_regular<sak::resource_pool<dummy_one>>::value);
    EXPECT_FALSE(is_regular<sak::resource_pool<dummy_two>>::value);
}

/// Test the basic API construct and free some objects
TEST(TestResourcePool, Api)
{
    {
        sak::resource_pool<dummy_one> pool;

        EXPECT_EQ(pool.unused_resources(), 0U);

        {
            auto d1 = pool.allocate();
            EXPECT_EQ(pool.unused_resources(), 0U);
        }

        EXPECT_EQ(pool.unused_resources(), 1U);

        auto d2 = pool.allocate();

        EXPECT_EQ(pool.unused_resources(), 0U);

        auto d3 = pool.allocate();

        EXPECT_EQ(pool.unused_resources(), 0U);
        EXPECT_EQ(dummy_one::m_count, 2);

        {
            auto d4 = pool.allocate();
            EXPECT_EQ(pool.unused_resources(), 0U);
        }

        EXPECT_EQ(pool.unused_resources(), 1U);

        pool.free_unused();

        EXPECT_EQ(pool.unused_resources(), 0U);
    }

    EXPECT_EQ(dummy_one::m_count, 0);
}

/// Test the pool works with std::bind
TEST(TestResourcePool, bind)
{
    {
        sak::resource_pool<dummy_one> pool_one(std::bind(make_dummy_one));
        sak::resource_pool<dummy_two> pool_two(std::bind(make_dummy_two, 4U));

        auto o1 = pool_one.allocate();
        auto o2 = pool_two.allocate();

        EXPECT_EQ(dummy_one::m_count, 1U);
        EXPECT_EQ(dummy_two::m_count, 1U);
    }

    EXPECT_EQ(dummy_one::m_count, 0U);
    EXPECT_EQ(dummy_two::m_count, 0U);
}


/// Test that the pool works for non default constructable objects, if
/// we provide the allocator
TEST(TestResourcePool, NonDefaultConstructable)
{
    {
        sak::resource_pool<dummy_two> pool(std::bind(make_dummy_two, 4U));

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

    EXPECT_EQ(dummy_two::m_count, 0U);
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
/// objects allocated
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

            // EXPECT_EQ(pool.total_resources(), 3U);
            EXPECT_EQ(dummy_one::m_count, 3U);
        }

        EXPECT_EQ(dummy_one::m_count, 3U);

    }

    EXPECT_EQ(dummy_one::m_count, 0U);
}

/// Test that the recycle functionality works
TEST(TestResourcePool, Recycle)
{
    uint32_t recycled = 0;

    auto recycle = [&recycled](std::shared_ptr<dummy_two> o)
        {
            EXPECT_TRUE((bool) o);
            ++recycled;
        };

    auto make = []()->std::shared_ptr<dummy_two>
        {
            return std::make_shared<dummy_two>(3U);
        };

    sak::resource_pool<dummy_two> pool(make, recycle);

    auto o1 = pool.allocate();
    o1.reset();

    EXPECT_EQ(recycled, 1U);
}

/// Test that copying the resource_pool works as expected.
///
/// For a type to be regular then:
///
///     T a = b; assert(a == b);
///     T a; a = b; <-> T a = b;
///     T a = c; T b = c; a = d; assert(b == c);
///     T a = c; T b = c; zap(a); assert(b == c && a != b);
///
TEST(TestResourcePool, CopyConstructor)
{
    sak::resource_pool<dummy_one> pool;

    auto o1 = pool.allocate();
    auto o2 = pool.allocate();

    o1.reset();

    sak::resource_pool<dummy_one> new_pool(pool);

    EXPECT_EQ(pool.unused_resources(), 1U);
    EXPECT_EQ(new_pool.unused_resources(), 1U);

    o2.reset();

    EXPECT_EQ(pool.unused_resources(), 2U);
    EXPECT_EQ(new_pool.unused_resources(), 1U);

    EXPECT_EQ(dummy_one::m_count, 3U);

    pool.free_unused();
    new_pool.free_unused();

    EXPECT_EQ(dummy_one::m_count, 0U);
}

/// Test copy assignment works
TEST(TestResourcePool, CopyAssignment)
{
    sak::resource_pool<dummy_one> pool;

    auto o1 = pool.allocate();
    auto o2 = pool.allocate();

    o1.reset();

    sak::resource_pool<dummy_one> new_pool;
    new_pool = pool;

    EXPECT_EQ(dummy_one::m_count, 3U);
    auto o3 = new_pool.allocate();
    EXPECT_EQ(dummy_one::m_count, 3U);
}

/// Test move constructor
TEST(TestResourcePool, MoveConstructor)
{
    sak::resource_pool<dummy_one> pool;

    auto o1 = pool.allocate();
    auto o2 = pool.allocate();

    o1.reset();

    sak::resource_pool<dummy_one> new_pool(std::move(pool));

    o2.reset();
    EXPECT_EQ(new_pool.unused_resources(), 2U);
}

/// Test move assignment
TEST(TestResourcePool, MoveAssignment)
{
    sak::resource_pool<dummy_one> pool;

    auto o1 = pool.allocate();
    auto o2 = pool.allocate();

    o1.reset();

    sak::resource_pool<dummy_one> new_pool;
    new_pool = std::move(pool);

    o2.reset();

    EXPECT_EQ(new_pool.unused_resources(), 2U);
}

/// Test that copy assignment works when we copy from an object with
/// recycle functionality
TEST(TestResourcePool, CopyRecycle)
{
    uint32_t recycled = 0;

    auto recycle = [&recycled](std::shared_ptr<dummy_two> o)
        {
            EXPECT_TRUE((bool) o);
            ++recycled;
        };

    auto make = []()->std::shared_ptr<dummy_two>
        {
            return std::make_shared<dummy_two>(3U);
        };

    sak::resource_pool<dummy_two> pool(make, recycle);
    sak::resource_pool<dummy_two> new_pool = pool;

    EXPECT_EQ(pool.unused_resources(), 0U);
    EXPECT_EQ(new_pool.unused_resources(), 0U);

    auto o1 = new_pool.allocate();

    EXPECT_EQ(dummy_two::m_count, 1U);

    o1.reset();
    EXPECT_EQ(recycled, 1U);

    new_pool.free_unused();

    EXPECT_EQ(dummy_two::m_count, 0U);
}
