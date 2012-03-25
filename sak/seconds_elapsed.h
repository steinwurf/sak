// Copyright Steinwurf APS 2011-2012.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#ifndef SAK_SECONDS_ELAPSED_H
#define SAK_SECONDS_ELAPSED_H

#include <boost/timer/timer.hpp>

namespace sak
{

    // Convenience function for boost::cpu_timer to get the CPU time
    // elapsed (user + system).
    // @todo: Timers can be replaced by C++0x chrono functionality
    //
    // @param cpu_timer, a boost::cpu_timer instance
    // @return the number of seconds elapsed
    inline long double seconds_elapsed(const boost::timer::cpu_timer &timer)
    {
        boost::timer::cpu_times elapsed_times = timer.elapsed();

        const long double sec = 1000000000.0L;

        boost::timer::nanosecond_type total =
            elapsed_times.system + elapsed_times.user;

        long double total_sec = total / sec;

        return total_sec;
    }

}

#endif
