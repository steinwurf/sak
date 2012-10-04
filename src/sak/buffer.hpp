// Copyright (c) 2011-2012 Steinwurf ApS
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

#ifndef SAK_BUFFER_HPP
#define SAK_BUFFER_HPP

#include <vector>

/// helper class that implement a buffer that can used in network code
/// data can be read into the buffer which appends it to the existing data
//in the buffer. Keeps track of the amount of data in the buffer.
class buffer
{
public:
    buffer();

    /// @param buffer_start the start of the buffer to be read/copied
    /// @param buffer_end the end of the buffer to be read/copied
    void write(const uint8_t *buffer_start, const uint8_t *buffer_end);

    /// @return a pointer to the start of the data in the buffer
    uint8_t* data();

    /// @return the size in bytes of the data in the buffer
    uint32_t size();

    /// Clear the data in the buffer
    void clear();

private:
    /// Internal storage
    std::vector<uint8_t> m_vector;

    /// The amount on internally stored data
    uint32_t m_size;
};

#endif
