// Copyright (c) 2011-2013 Steinwurf ApS
// All Rights Reserved

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

#pragma once

#include <stdint.h>
#include <cassert>

#include <vector>
#include <string>
#include <algorithm>

#include "convert_endian.hpp"

namespace sak
{

    /// The idea behind the endian_stream is to provide a simple interface for
    /// writing to and reading from an std::vector. 
    /// All complexity regarding endianness is encapsulated.
    class endian_stream
    {

    public:

        /// Creates an endian buffer on top of a preallocated buffer of the
        /// specified size
        /// @param buffer a pointer to the buffer
        /// @param size the i size of the buffer
        endian_stream(std::vector<uint8_t>& vector, uint32_t offset = 0);

        /// Writes a value of the size of ValueType to the buffer
        /// @param value the value to write
        template<class ValueType>
        void write(ValueType value)
        {           
            // Make sure there is enough space in the underlying vector
            if (m_vector.size() < m_position + sizeof(ValueType))
                m_vector.resize(m_position + sizeof(ValueType));
            // Write the value at the current position
            big_endian::put<ValueType>(value, &m_vector[m_position]);
            // Advance the current position
            m_position += sizeof(ValueType);
        }

        template<class LengthType>
        inline void write_string(const std::string& value)
        {
            write<LengthType>(value.size());
            write_string(value);
        }

        inline void write_string(const std::string& value)
        {            
            uint32_t bytes = value.size();
            if (bytes == 0) return;
            // Make sure there is enough space in the underlying vector
            if (m_vector.size() < m_position + bytes)
                m_vector.resize(m_position + bytes);
            // Write the value at the current position
            // Copy the string to the buffer
            std::copy_n(value.begin(), bytes, &m_vector[m_position]);
            // Advance the current position
            m_position += bytes;            
        }

        inline void write_vector(const std::vector<uint8_t>& value)
        {
            uint32_t bytes = value.size();
            if (bytes == 0) return;
            // Make sure there is enough space in the underlying vector
            if (m_vector.size() < m_position + bytes)
                m_vector.resize(m_position + bytes);
            // Write the value at the current position
            // Copy the string to the buffer
            std::copy_n(value.begin(), bytes, &m_vector[m_position]);
            // Advance the current position
            m_position += bytes;            
        }
        

        /// Reads from the buffer and moves the write position.
        /// @param value reference to the value to be read
        /// @return boolean to indicate if the reading operation was successful
        template<class ValueType>
        bool read(ValueType& value)
        {
            // Make sure there is enough data to read in the underlying vector
            if (m_vector.size() < m_position + sizeof(ValueType))
                return false;
            // Read the value at the current position
            value = big_endian::get<ValueType>(&m_vector[m_position]);
            // Advance the current position
            m_position += sizeof(ValueType);
            return true;
        } 

        template<class LengthType>
        inline bool read_string(std::string& value)
        {
            typename LengthType length;
            if (read<LengthType>(length)==false) return false; 
            return read_string(value, length);
        }

        inline bool read_string(std::string& value, uint32_t bytes)
        {
            if (bytes == 0) return true;
            // Make sure there is enough data to read in the underlying vector
            if (m_vector.size() < m_position + bytes)
                return false;
            
            // Reserve space in the target buffer
            value.resize(bytes);
            // Read the string from the buffer
            std::copy_n(&m_vector[m_position], bytes, value.begin());
            // Advance the current position
            m_position += bytes;
            return true;
        }

        inline bool read_vector(std::vector<uint8_t>& value, uint32_t bytes)
        {
            if (bytes == 0) return true;
            // Make sure there is enough data to read in the underlying vector
            if (m_vector.size() < m_position + bytes)
                return false;
            
            // Reserve space in the target buffer
            value.resize(bytes);
            // Read the string from the buffer
            std::copy_n(&m_vector[m_position], bytes, value.begin());
            // Advance the current position
            m_position += bytes;
            return true;
        }


        /// Gets the current read/write position of the buffer
        /// @return the current position
        uint32_t position() const;

    private:

        /// Pointer to the buffer
        std::vector<uint8_t>& m_vector;

        /// The current position
        uint32_t m_position;
        
    };
}
