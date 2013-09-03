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

#include <cstdint>
#include <ctime>

#include <gtest/gtest.h>

#include <sak/buffer_input_stream.hpp>

TEST(TestBufferInputStream, CreateBufferInputStream)
{
    // Test: read from one buffer to another

    srand(static_cast<uint32_t>(time(0)));

    {
        // Ensure that  buffer size is greater than 2.
        uint32_t buffer_size = (rand() % 100000) + 3;

        std::vector<char> buffer(buffer_size, '\0');

        for (uint32_t i = 0; i < buffer_size; ++i)
        {
            buffer[i] = (rand() % 255);
        }

        // First: Implementation through const_storage constructor

        const uint8_t* my_data = reinterpret_cast<const uint8_t*>(&buffer[0]);

        sak::const_storage my_stor(my_data,buffer_size);
        sak::buffer_input_stream input_stream (my_stor);

        ASSERT_TRUE( input_stream.size() == buffer_size );
        ASSERT_TRUE( input_stream.read_position() == 0 );


        // Second: Implementation through storage conversion function


        // buffer_b is another test buffer
        std::vector<char> buffer_b(buffer_size, '\0');

        // Conversion from buffer_b to my_storage_2
        sak::const_storage my_storage_2 = sak::storage(buffer_b);

        sak::buffer_input_stream input_stream_2 (my_storage_2);

        ASSERT_TRUE( input_stream_2.size() == buffer_size );
        ASSERT_TRUE( input_stream_2.read_position() == 0 );



        // Reading from first storage
        std::vector<char> buffer_out;

        while ( input_stream.bytes_available() > 0 )
        {
            // Random read (always positive thus + 1)
            uint32_t read_request = (rand() % 100) + 1;

            uint32_t read = std::min(read_request,
                                     input_stream.bytes_available());

            std::vector<char> read_buffer(read, '\0');

            input_stream.read(reinterpret_cast<uint8_t*>(read_buffer.data()),
                              read);

            buffer_out.insert(buffer_out.end(), read_buffer.begin(),
                              read_buffer.end());
        }

        ASSERT_TRUE(std::equal(buffer.begin(), buffer.end(),
                    buffer_out.begin()));

        //Check the positions "buffer_size - 2" read from input and output
        //buffer 1 with "seek" function

        input_stream.seek(buffer_size - 2);


        const uint32_t m_data_position = input_stream.read_position();


        ASSERT_EQ(buffer_size - 2,m_data_position);

        // Reading from second storage

        std::vector<char> buffer_out_2;

        while ( input_stream_2.bytes_available() > 0 )
        {
            // Random read (always positive thus + 1)
            uint32_t read_request_2 = (rand() % 100) + 1;

            uint32_t read_2 = std::min(read_request_2,
                                     input_stream_2.bytes_available());

            std::vector<char> read_buffer_2(read_2, '\0');

            input_stream_2.read(reinterpret_cast<uint8_t*>(
                                read_buffer_2.data()),read_2);

            buffer_out_2.insert(buffer_out_2.end(), read_buffer_2.begin(),
                              read_buffer_2.end());
        }

        ASSERT_TRUE(std::equal(buffer_b.begin(), buffer_b.end(),
                    buffer_out_2.begin()));

        //Check that the input streams have "stopped"
        //This only checks that the functions has been properly called
        //since finite input streams will always stopped when they are readed

        ASSERT_TRUE( input_stream.stopped() == true );
        ASSERT_TRUE( input_stream_2.stopped() == true );

    }

}
