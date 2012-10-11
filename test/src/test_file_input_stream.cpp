#include <gtest/gtest.h>

#include <stdint.h>
#include <ctime>
#include <algorithm>
#include <fstream>

#include <sak/file_input_stream.hpp>


/// Tests reading a file, the file is crated a priori
TEST(TestFileInputStream, ReadRandomFile)
{
    srand(static_cast<uint32_t>(time(0)));

    {
        uint32_t file_size = 1000000;
        std::string file_name("test.txt");

        std::vector<char> output_buffer(file_size, '\0');

        for(uint32_t i = 0; i < file_size; ++i)
        {
            output_buffer[i] = (rand() % 255);
        }

        std::ofstream output_file(file_name.c_str(),
                                  std::ios::out | std::ios::binary);
        output_file.write(&output_buffer[0], file_size);
        output_file.close();


        // Now test we can read it back
        sak::file_input_stream fs;

        fs.open(file_name);

        ASSERT_EQ(file_size, fs.bytes_available());

        uint32_t read_size = 512;

        std::vector<char> input_buffer;

        while(fs.bytes_available() > 0)
        {
            uint32_t read = std::min(read_size, fs.bytes_available());

            ASSERT_TRUE(read <= read_size);

            std::vector<char> temp(read, '\0');
            fs.read(reinterpret_cast<uint8_t*>(&temp[0]), read);

            input_buffer.insert(input_buffer.end(),
                                temp.begin(),
                                temp.end());

        }

        bool result = std::equal( input_buffer.begin(),
                                  input_buffer.end(),
                                  output_buffer.begin() );


        ASSERT_TRUE(result);

    }
}

/// Tests error handling with exception
TEST(TestFileInputStream, ExceptionThrow)
{

    sak::file_input_stream fs;
    boost::system::error_code ec;

    try
    {
        fs.open("strange_file_that_should_not_exist.notfound");
    }
    catch(const boost::system::system_error &error)
    {
        ec = error.code();
    }

    EXPECT_EQ(ec, sak::error::failed_open_file);

}

/// Tests error handling with exception
TEST(TestFileInputStream, ExceptionReturn)
{

    sak::file_input_stream fs;

    boost::system::error_code ec;
    fs.open("strange_file_that_should_not_exist.notfound", ec);

    EXPECT_EQ(ec, sak::error::failed_open_file);

}

/// Tests error handling with exception in constructor
TEST(TestFileInputStream, ExceptionThrowConstructor)
{


    boost::system::error_code ec;

    try
    {
        sak::file_input_stream fs(
            "strange_file_that_should_not_exist.notfound");
    }
    catch(const boost::system::system_error &error)
    {
        ec = error.code();
    }

    EXPECT_EQ(ec, sak::error::failed_open_file);

}



