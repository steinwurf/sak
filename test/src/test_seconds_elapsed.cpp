// Copyright (c) 2012, Steinwurf ApS
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of Steinwurf ApS nor the
//       names of its contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


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


