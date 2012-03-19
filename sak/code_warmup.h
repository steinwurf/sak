// Copyright (c) 2012, Steinwurf ApS
// All rights reserved.

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

#ifndef SAK_CODE_WARMUP_H
#define SAK_CODE_WARMUP_H

#include "seconds_elapsed.h"

namespace sak
{

    // Helper class which can be used when profiling code.
    //
    // Example:
    //
    //    // Warm up
    //    code_warmup warmup;
    //
    //    while(!warmup.done())
    //    {
    //        perform_calculation();
    //        warmup.next_iteration();
    //    }
    //
    //    // Target time of benchmark in seconds
    //    long double target_time = 5.0;
    //
    //    uint32_t needed_iterations = warmup.iterations(target_time);
    //
    //    // Now in e.g. a for loop and timing with a timer
    //
    //    boost::timer::cpu_timer timer;
    //    timer.start();
    //
    //    for(uint32_t j = 0; j < needed_iterations; ++j)
    //    {
    //        perform_calculation();
    //    }
    //
    //    timer.stop();
    //
    //    long double total_sec = sak::seconds_elapsed(timer);
    //
    // In short the warmup class runs the code for a certain amount
    // of time while counting the iterations. Based on this it calculates
    // the number of iterations needed to benchmark for a specific amount
    // of time. Note, that following the recommendations of:
    // http://www.boost.org/doc/libs/1_49_0/libs/timer/doc/cpu_timers.html
    //
    // We should always measure at least 100 times longer than the CPU timer
    // resolution. See the above mentioned page for more info.
    //
    //
    class code_warmup
    {
    public:

        // Construct a new code_warmup object
        // @param warmup_time, the time in seconds before the warmup
        //        object will return true in the done() function
        code_warmup(long double warmup_time = 1.0);

        // Based on the elapsed time between start() and stop()
        // returns the number of iterations needed to reach the
        // target time.
        // @parma target_time, the time in seconds we wich our
        //        code to run
        // @return the number of iterations
        uint32_t iterations(long double target_time) const;

        // Increment the iterations counter, should be called
        // every time the code being measured completes one
        // iteration.
        void next_iteration();

        // @returns true if the warmup is complete
        bool done();

    private:

        // The desired time spent during warmup
        long double m_warmup_time;

        // Keeping track of the iterations
        uint32_t m_iterations;

        // The stop watch used
        boost::timer::cpu_timer m_timer;

    };

    code_warmup::code_warmup(long double warmup_time)
        : m_warmup_time(warmup_time),
          m_iterations(0)
    {
        assert(m_warmup_time > 0);
        m_timer.start();
    }


    uint32_t code_warmup::iterations(long double target_time) const
    {
        long double time_spent = seconds_elapsed(m_timer);

        assert(time_spent > 0);
        assert(m_iterations > 0);

        if(target_time < time_spent)
        {
            return m_iterations;
        }
        else
        {
            // ceil(x/y) = ((x - 1) / y) + 1
            long double factor = ((target_time - 1) / time_spent ) + 1;

            assert(factor > 0);

            return m_iterations * factor;
        }
    }

    void code_warmup::next_iteration()
    {
        assert(!m_timer.is_stopped());
        ++m_iterations;
    }

    bool code_warmup::done()
    {
        assert(!m_timer.is_stopped());

        long double sec = seconds_elapsed(m_timer);

        bool time_exceeded = sec > m_warmup_time;

        if(time_exceeded)
        {
            m_timer.stop();
        }

        return time_exceeded;
    }

}

#endif

