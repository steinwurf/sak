// Copyright (c) 2014 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <cstdint>
#include <vector>
#include <string>

#include <gtest/gtest.h>

#include <sak/easy_bind.hpp>

namespace
{
    uint32_t global_a = 0;
    uint32_t global_b = 0;
    uint32_t global_c = 0;

    void clear_state()
    {
        global_a = 0;
        global_b = 0;
        global_c = 0;
    }

    void free_function(uint32_t a, uint32_t b, uint32_t c)
    {
        global_a = a;
        global_b = b;
        global_c = c;
    }

    struct dummy_class
    {
        void method(uint32_t x, double y, std::string str)
        {
            m_x = x;
            m_y = y;
            m_str = str;
        }

        uint32_t m_x;
        double m_y;
        std::string m_str;
    };
}

TEST(TestEasyBind, test_free_function)
{
    uint32_t a = 1;
    uint32_t b = 2;
    uint32_t c = 3;

    clear_state();
    auto function1 = sak::easy_bind(&free_function);
    function1(a, b, c);
    EXPECT_EQ(a, global_a);
    EXPECT_EQ(b, global_b);
    EXPECT_EQ(c, global_c);

    clear_state();
    auto function2 = sak::easy_bind(&free_function, a);
    function2(b, c);
    EXPECT_EQ(a, global_a);
    EXPECT_EQ(b, global_b);
    EXPECT_EQ(c, global_c);

    clear_state();
    auto function3 = sak::easy_bind(&free_function, a, b);
    function3(c);
    EXPECT_EQ(a, global_a);
    EXPECT_EQ(b, global_b);
    EXPECT_EQ(c, global_c);

    clear_state();
    auto function4 = sak::easy_bind(&free_function, a, b, c);
    function4();
    EXPECT_EQ(a, global_a);
    EXPECT_EQ(b, global_b);
    EXPECT_EQ(c, global_c);
}

TEST(TestEasyBind, test_member_function)
{
    dummy_class* dummy = new dummy_class();

    auto function1 = sak::easy_bind(&dummy_class::method, dummy);
    function1(1, 1.5, "test1");
    EXPECT_EQ(1, dummy->m_x);
    EXPECT_EQ(1.5, dummy->m_y);
    EXPECT_EQ("test1", dummy->m_str);

    delete dummy;
}
