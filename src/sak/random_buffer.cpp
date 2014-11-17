// Copyright (c) 2012 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include "random_buffer.hpp"

#include <ctime>
#include <cassert>

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>

#include "convert_endian.hpp"

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
