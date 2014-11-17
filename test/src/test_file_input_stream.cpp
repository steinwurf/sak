// Copyright (c) 2012 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <sak/file_input_stream.hpp>

#include <cstdio>
#include <cstdint>
#include <ctime>
#include <algorithm>
#include <fstream>

#include <gtest/gtest.h>

/// Tests reading a file, the file is crated a priori
TEST(TestFileInputStream, ReadRandomFile)
{
    // Create a file with random contents
    uint32_t file_size = 1000;
    std::string file_name("test.txt");

    std::vector<char> output_buffer(file_size, '\0');

    for (uint32_t i = 0; i < file_size; ++i)
    {
        output_buffer[i] = (rand() % 255);
    }

    std::ofstream output_file(
        file_name.c_str(), std::ios::out | std::ios::binary);

    ASSERT_TRUE(output_file.is_open());

    output_file.write(&output_buffer[0], file_size);
    output_file.close();

    // Now test we can read it back
    sak::file_input_stream fs;

    boost::system::error_code ec;

    fs.open(file_name, ec);
    ASSERT_FALSE(ec);

    ASSERT_EQ(file_size, fs.bytes_available());

    uint32_t read_size = 512;

    std::vector<char> input_buffer;

    while (fs.bytes_available() > 0)
    {
        uint32_t read = std::min(read_size, fs.bytes_available());

        ASSERT_TRUE(read <= read_size);

        std::vector<char> temp(read, '\0');
        fs.read(reinterpret_cast<uint8_t*>(&temp[0]), read);

        input_buffer.insert(
            input_buffer.end(), temp.begin(), temp.end());
    }
    // Always close the input file stream
    fs.close();

    bool result = std::equal(
        input_buffer.begin(), input_buffer.end(), output_buffer.begin());

    ASSERT_TRUE(result);

    // Make sure that test.txt is removed after the test
    EXPECT_EQ(0, std::remove(file_name.c_str()));
}

/// Tests error handling with exception
TEST(TestFileInputStream, ThrowExceptionInOpen)
{
    sak::file_input_stream fs;
    boost::system::error_code ec;

    try
    {
        fs.open("strange_file_that_should_not_exist.notfound");
    }
    catch (const boost::system::system_error& error)
    {
        ec = error.code();
    }

    EXPECT_EQ(ec, sak::error::failed_open_file);
}

/// Tests error handling with error code
TEST(TestFileInputStream, ReturnErrorCode)
{
    sak::file_input_stream fs;

    boost::system::error_code ec;
    fs.open("strange_file_that_should_not_exist.notfound", ec);

    EXPECT_EQ(ec, sak::error::failed_open_file);
}

/// Tests error handling with exception in constructor
TEST(TestFileInputStream, ThrowExceptionInConstructor)
{
    boost::system::error_code ec;

    try
    {
        sak::file_input_stream fs(
            "strange_file_that_should_not_exist.notfound");
    }
    catch (const boost::system::system_error& error)
    {
        ec = error.code();
    }

    EXPECT_EQ(ec, sak::error::failed_open_file);
}
