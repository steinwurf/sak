// Copyright (c) 2012-2013, Steinwurf ApS
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
// DISCLAIMED. IN NO EVENT SHALL Steinwurf ApS BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include <ctime>
#include <cassert>

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>

#include "convert_endian.hpp"

#include "random_buffer.hpp"

namespace sak
{

    void random_buffer::fill(uint32_t size, bool embed_seed)
    {
        buffer::resize(size);

        // Random generator used to fill the buffer
        boost::random::mt19937 rng;
        // Uniform distribution for uint8_t type with range: [0, 255]
        boost::random::uniform_int_distribution<uint8_t> dist;
        uint32_t seed = (uint32_t)std::time(0);
        rng.seed(seed);

        uint32_t start = 0;
        uint8_t* buffer = buffer::data();

        if (embed_seed)
        {
            assert(size > sizeof(uint32_t));
            // Write the random seed to the beginning of the buffer
            big_endian::put<uint32_t>(seed, buffer);
            start = sizeof(uint32_t);
        }

        for (uint32_t i = start; i < size; ++i)
        {
            buffer[i] = dist(rng);
        }
    }


    bool random_buffer::verify()
    {
        uint32_t size = buffer::size();
        assert(size > sizeof(uint32_t));

        uint8_t* buffer = buffer::data();

        // Read the random seed from the beginning of the buffer
        uint32_t seed = big_endian::get<uint32_t>(buffer);

        // Random generator used to verify the contents
        boost::random::mt19937 rng;
        // Uniform distribution for uint8_t type with range: [0, 255]
        boost::random::uniform_int_distribution<uint8_t> dist;
        // Use the embedded seed
        rng.seed(seed);

        uint32_t start = sizeof(uint32_t);

        for (uint32_t i = start; i < size; ++i)
        {
            // Test each byte against the random generated values
            if (buffer[i] != dist(rng)) return false;
        }
        return true;
    }

}
