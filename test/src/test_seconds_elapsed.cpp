// Copyright Steinwurf APS 2011-2012.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing


#include <gtest/gtest.h>
#include <sak/seconds_elapsed.h>

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



TEST(SecondsElapsed, StartStopResume)
{

    boost::timer::cpu_timer timer;

    timer.start();

    while(sak::seconds_elapsed(timer) < 1.0)
    {
        volatile std::string s = burn_time();
    }

    timer.stop();

    long double time_elapsed = sak::seconds_elapsed(timer);

    EXPECT_TRUE(time_elapsed >= 1.0);

    timer.resume();

    while(sak::seconds_elapsed(timer) < 2.0)
    {
        EXPECT_TRUE(time_elapsed >= 1.0);
        volatile std::string s = burn_time();
    }

    timer.stop();

    time_elapsed = sak::seconds_elapsed(timer);

    EXPECT_TRUE(time_elapsed >= 2.0);

}


