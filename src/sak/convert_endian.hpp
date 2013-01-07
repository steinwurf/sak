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
// DISCLAIMED. IN NO EVENT SHALL Steinwurf ApS BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef SAK_CONVERT_ENDIAN_H
#define SAK_CONVERT_ENDIAN_H

#include <cstdint>
#include <cassert>
#include <boost/detail/endian.hpp>

namespace sak
{

    // This struct defines a bool which defines whether the
    // platform endiannes is big or little.
    // We rely on the macros defined in Boost
    struct host_endian
    {
        static const bool big_endian =
            #ifdef BOOST_BIG_ENDIAN
                true;
            #else
                false;
            #endif
    };


    template<bool Swap>
    struct convert_endian;

    template<>
    struct convert_endian<true>
    {

        static void put16(uint16_t v, uint8_t *b)
            {
                b[1] = (v & 0xFF); b[0] = (v >> 8 & 0xFF);
            }

        static void put32(uint32_t v, uint8_t *b)
            {
                b[3] = (v & 0xFF);         b[2] = ((v >> 8) & 0xFF);
                b[1] = ((v >> 16) & 0xFF); b[0] = ((v >> 24) & 0xFF);
            }

        static void put64(uint64_t v, uint8_t *b)
            {
                b[7] = (v & 0xFF);         b[6] = ((v >> 8) & 0xFF);
                b[5] = ((v >> 16) & 0xFF); b[4] = ((v >> 24) & 0xFF);
                b[3] = ((v >> 32) & 0xFF); b[2] = ((v >> 40) & 0xFF);
                b[1] = ((v >> 48) & 0xFF); b[0] = ((v >> 56) & 0xFF);
            }

        static uint16_t get16(const uint8_t *b)
            {
                return (b[0] << 8) | b[1];
            }

        static uint32_t get32(const uint8_t *b)
            {
                return (b[0] << 24) | (b[1] << 16) | (b[2] << 8) | b[3];
            }

        static uint64_t get64(const uint8_t *b)
            {
                return (((uint64_t) b[0]) << 56) | (((uint64_t) b[1]) << 48) |
                       (((uint64_t) b[2]) << 40) | (((uint64_t) b[3]) << 32) |
                       (((uint64_t) b[4]) << 24) | (((uint64_t) b[5]) << 16) |
                       (((uint64_t) b[6]) << 8)  | ((uint64_t) b[7]);
            }
    };

    template<>
    struct convert_endian<false>
    {
        static void put16(uint16_t v, uint8_t *b)
            {
                b[0] = (v & 0xFF); b[1] = (v >> 8 & 0xFF);
            }

        static void put32(uint32_t v, uint8_t *b)
            {
                b[0] = (v & 0xFF);         b[1] = ((v >> 8) & 0xFF);
                b[2] = ((v >> 16) & 0xFF); b[3] = ((v >> 24) & 0xFF);
            }

        static void put64(uint64_t v, uint8_t *b)
            {
                b[0] = (v & 0xFF);         b[1] = ((v >> 8) & 0xFF);
                b[2] = ((v >> 16) & 0xFF); b[3] = ((v >> 24) & 0xFF);
                b[4] = ((v >> 32) & 0xFF); b[5] = ((v >> 40) & 0xFF);
                b[6] = ((v >> 48) & 0xFF); b[7] = ((v >> 56) & 0xFF);
            }

        static uint16_t get16(const uint8_t *b)
            {
                return (b[1] << 8) | b[0];
            }

        static uint32_t get32(const uint8_t *b)
            {
                return (b[3] << 24) | (b[2] << 16) | (b[1] << 8) | b[0];
            }

        static uint64_t get64(const uint8_t *b)
            {
                return (((uint64_t) b[7]) << 56) | (((uint64_t) b[6]) << 48) |
                       (((uint64_t) b[5]) << 40) | (((uint64_t) b[4]) << 32) |
                       (((uint64_t) b[3]) << 24) | (((uint64_t) b[2]) << 16) |
                       (((uint64_t) b[1]) << 8)  | ((uint64_t) b[0]);
            }



    };

    // Inserts and extracts integers in big-endian format.
    struct big_endian
    {
        /// If the host is not big endian swap becomes true
        /// and all values read or written will be swapped
        typedef convert_endian<host_endian::big_endian == false> convert;

        /// Gets an 8-bit value integer from a byte stream. Only exists for
        /// convince in the template based getters and putters!!
        /// @param buffer pointer to the byte stream buffer
        /// @return retrieved value from the byte stream
        static uint8_t get8(const uint8_t *buffer)
            {
                assert(buffer != 0);
                return *buffer;
            }

        /// Inserts an 8-bit value integer into the byte stream. Only exists for
        /// convince in the template based getters and putters
        /// @param value to put in the stream
        /// @param buffer pointer to the byte stream buffer
        static void put8(uint8_t value, uint8_t *buffer)
            {
                assert(buffer != 0);
                *buffer = value;
            }

        /// Gets a 16-bit value integer which is in big-endian format from a byte
        /// stream.
        /// @copydetails get8()
        static uint16_t get16(const uint8_t *buffer)
            {
                assert(buffer != 0);
                return convert::get16(buffer);
            }

        /// Inserts a 16-bit value into a byte stream in big-endian format.
        /// @copydetails put8()
        static void put16(uint16_t value, uint8_t *buffer)
            {
                assert(buffer != 0);
                convert::put16(value, buffer);
            }

        /// Gets a 32-bit value integer which is in big-endian format from a
        /// byte stream.
        /// @copydetails get8()
        static uint32_t get32(const uint8_t *buffer)
            {
                assert(buffer != 0);
                return convert::get32(buffer);
            }

        /// Inserts a 32-bit value into a byte stream in big-endian format.
        /// @copydetails put8()
        static void put32(uint32_t value, uint8_t *buffer)
            {
                assert(buffer != 0);
                convert::put32(value, buffer);
            }

        /// Gets a 64-bit value integer which is in big-endian format from a
        /// byte stream.
        /// @copydetails get8()
        static uint64_t get64(const uint8_t *buffer)
            {
                assert(buffer != 0);
                return convert::get64(buffer);
            }

        /// Inserts a 64-bit value into a byte stream in big-endian format.
        /// @copydetails put8()
        static void put64(uint64_t value, uint8_t *buffer)
            {
                assert(buffer != 0);
                convert::put64(value, buffer);
            }

        /// Template based put and get functions, the main reason for these is
        /// to allow generic code to be written where the "right" get/put
        /// function will be called based on the a template determined data-type
        template<class ValueType>
        static void put(ValueType value, uint8_t *buffer);

        template<class ValueType>
        static ValueType get(const uint8_t *buffer);

    };

    template<>
    inline void big_endian::put<uint8_t>(uint8_t value, uint8_t *buffer)
    {
        big_endian::put8(value, buffer);
    }

    template<>
    inline void big_endian::put<uint16_t>(uint16_t value, uint8_t *buffer)
    {
        big_endian::put16(value, buffer);
    }

    template<>
    inline void big_endian::put<uint32_t>(uint32_t value, uint8_t *buffer)
    {
        big_endian::put32(value, buffer);
    }

    template<>
    inline void big_endian::put<uint64_t>(uint64_t value, uint8_t *buffer)
    {
        big_endian::put64(value, buffer);
    }

    template<>
    inline uint8_t big_endian::get<uint8_t>(const uint8_t *buffer)
    {
        return big_endian::get8(buffer);
    }

    template<>
    inline uint16_t big_endian::get<uint16_t>(const uint8_t *buffer)
    {
        return big_endian::get16(buffer);
    }

    template<>
    inline uint32_t big_endian::get<uint32_t>(const uint8_t *buffer)
    {
        return big_endian::get32(buffer);
    }

    template<>
    inline uint64_t big_endian::get<uint64_t>(const uint8_t *buffer)
    {
        return big_endian::get64(buffer);
    }


}




#endif

