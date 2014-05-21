// Copyright (c) 2014 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <functional>
#include <gtest/gtest.h>

#include <sak/easy_bind.hpp>
#include <sak/optional_bind.hpp>

namespace
{
    uint32_t free_function(uint32_t a, uint32_t b)
    {
        return a + b;
    }

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

        uint32_t m_a;
        uint32_t m_b;
    };

    struct bind_method
    {
        template<class T>
        static auto bind(T& t) -> decltype(sak::easy_bind(&T::method, &t))
        {
            return sak::easy_bind(&T::method, &t);
        }
    };

    struct bind_no_method
    {
        template<class T>
        static auto bind(T& t) -> decltype(sak::easy_bind(&T::no_method, &t))
        {
            return sak::easy_bind(&T::no_method, &t);
        }
    };
}

// TEST(TestEasyBind, test_free_function)
// {
//     uint32_t a = 1;
//     uint32_t b = 2;



// }

TEST(TestOptionalBind, test_member_function)
{
    // We should also be able to use shared_ptr lets test this also
    // std::shared_ptr<dummy_class> dummy(new dummy_class());

    {
        dummy_class dummy;
        auto f1 = sak::optional_bind<bind_method>(dummy);
        auto f2 = sak::optional_bind<bind_no_method>(dummy);

        EXPECT_TRUE(sak::is_bind_expression(f1));
        EXPECT_FALSE(sak::is_bind_expression(f2));
    }

}



// TEST(TestEasyBind, test_std_function)
// {
//     uint32_t a = 1;
//     uint32_t b = 2;
//     uint32_t c = 3;

//     using namespace std::placeholders;

//     clear_state();

//     std::function<void(uint32_t,uint32_t,uint32_t)> f1 =
//         std::bind(&free_function, _1, _2, _3);

//     auto f2 = sak::easy_bind(f1);
//     f2(a, b, c);
//     EXPECT_EQ(a, global_a);
//     EXPECT_EQ(b, global_b);
//     EXPECT_EQ(c, global_c);

//     clear_state();

//     std::function<void(uint32_t)> f3 =
//         std::bind(&free_function, 10U, _1, 15U);

//     auto f4 = sak::easy_bind(f3);
//     f4(b);
//     EXPECT_EQ(10U, global_a);
//     EXPECT_EQ(b, global_b);
//     EXPECT_EQ(15U, global_c);
// }
