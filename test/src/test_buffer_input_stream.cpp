#include <gtest/gtest.h>

#include <cstdint>
#include <ctime>

#include <sak/buffer_input_stream.hpp>

TEST(TestBufferInputStream, CreateBufferInputStream)
{
    // Test: read from one buffer to another

    srand(static_cast<uint32_t>(time(0)));

    {
        uint32_t buffer_size = rand() % 100000;

	std::vector<char> buffer(buffer_size, '\0');

        for(uint32_t i = 0; i < buffer_size; ++i)
        {
            buffer[i] = (rand() % 255);
        }

        boost::shared_ptr<sak::buffer_input_stream> input_stream
            = sak::make_buffer_input( buffer );

        ASSERT_TRUE( input_stream->size() == buffer_size );
        ASSERT_TRUE( input_stream->read_position() == 0 );

	std::vector<char> buffer_out;

        while( input_stream->bytes_available() > 0 )
        {
            // Random read (always positive thus + 1)
            uint32_t read_request = (rand() % 100) + 1;

            uint32_t read = std::min(read_request, input_stream->bytes_available());

	    std::vector<char> read_buffer(read, '\0');

            input_stream->read( reinterpret_cast<uint8_t*>(read_buffer.data()), read );

	    buffer_out.insert(buffer_out.end(), read_buffer.begin(), read_buffer.end());
        }

        ASSERT_TRUE(std::equal(buffer.begin(), buffer.end(), buffer_out.begin()));

    }

}

