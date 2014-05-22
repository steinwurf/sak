// Copyright (c) 2014 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <functional>
#include <memory>

#include <gtest/gtest.h>

#include <sak/easy_bind.hpp>
#include <sak/optional_bind.hpp>

namespace
{
    struct dummy_class
    {
        dummy_class() :
            m_a(0), m_b(0)
        { }

        void method(uint32_t a, uint32_t b)
        {
            m_a = a;
            m_b = b;
        }

        uint32_t const_method(uint32_t a, uint32_t b) const
        {
            return a + b;
        }

        uint32_t m_a;
        uint32_t m_b;
    };

    struct bind_method
    {
        template<class T>
        static auto bind(T* t) ->
            decltype(sak::easy_bind(&T::method, t))
        {
            return sak::easy_bind(&T::method, t);
        }

        template<class T>
        static auto bind(T& t) ->
            decltype(sak::easy_bind(&T::method, &t))
        {
            return sak::easy_bind(&T::method, &t);
        }

        template<class T>
        static auto bind(const std::shared_ptr<T>& t) ->
            decltype(sak::easy_bind(&T::method, t))
        {
            return sak::easy_bind(&T::method, t);
        }
    };

    struct bind_const_method
    {
        template<class T, class... Args>
        static auto bind(const T* t) ->
            decltype(sak::easy_bind(&T::const_method, t))
        {
            return sak::easy_bind(&T::const_method, t);
        }
    };

    struct bind_no_method
    {
        template<class T>
        static auto bind(T* t) ->
            decltype(sak::easy_bind(&T::no_method, t))
        {
            return sak::easy_bind(&T::no_method, t);
        }
    };
}

TEST(TestOptionalBind, test_member_function)
{
    // Try with a pointer and a reference
    {
        dummy_class dummy;

        auto f1 = sak::optional_bind<bind_method>(&dummy);
        EXPECT_TRUE(sak::is_bind_expression(f1));

        f1(2,3);
        EXPECT_EQ(dummy.m_a, 2U);
        EXPECT_EQ(dummy.m_b, 3U);

        auto f2 = sak::optional_bind<bind_method>(dummy);
        EXPECT_TRUE(sak::is_bind_expression(f2));

        f2(4,5);
        EXPECT_EQ(dummy.m_a, 4U);
        EXPECT_EQ(dummy.m_b, 5U);

    }

    // Try with a shared_ptr
    {
        auto dummy = std::make_shared<dummy_class>();

        auto f1 = sak::optional_bind<bind_method>(dummy);
        EXPECT_TRUE(sak::is_bind_expression(f1));

        f1(2,3);
        EXPECT_EQ(dummy->m_a, 2U);
        EXPECT_EQ(dummy->m_b, 3U);
    }

    // Bind a const method though a const and non-const pointer
    {
        dummy_class dummy;

        dummy_class* non_const_dummy = &dummy;
        const dummy_class* const_dummy = &dummy;

        auto f1 = sak::optional_bind<bind_const_method>(non_const_dummy);
        EXPECT_TRUE(sak::is_bind_expression(f1));
        EXPECT_EQ(5U, f1(2,3));

        auto f2 = sak::optional_bind<bind_const_method>(const_dummy);
        EXPECT_TRUE(sak::is_bind_expression(f2));
        EXPECT_EQ(7U, f2(3,4));
    }

    // Try to bind to a method that does not exist
    {
        dummy_class dummy;

        auto f1 = sak::optional_bind<bind_no_method>(&dummy);
        EXPECT_FALSE(sak::is_bind_expression(f1));
    }
}
