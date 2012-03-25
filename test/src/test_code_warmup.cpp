// Copyright Steinwurf APS 2011-2012.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <gtest/gtest.h>
#include <sak/code_warmup.h>

#include <sstream>

static std::string burn_time()
{
    std::stringstream ss;
    for(uint32_t i = 0; i < 100000; ++i)
    {
        ss << static_cast<uint32_t>(rand());
    }

    return ss.str();
}


TEST(CodeWarmup, WarmupCode)
{

    // Warm up
    sak::code_warmup warmup;

    while(!warmup.done())
    {
        volatile std::string s = burn_time();
        warmup.next_iteration();
    }

    // Target time of benchmark in seconds
    long double target_time = 5.0;

    uint32_t needed_iterations = warmup.iterations(target_time);

    EXPECT_TRUE(needed_iterations > 0);

}



