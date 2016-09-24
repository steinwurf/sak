// Copyright (c) 2012 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>
#include <string>
#include <fstream>
#include <system_error>

#include "error.hpp"
#include "finite_input_stream.hpp"

namespace sak
{
/// A file input stream for reading local
/// files. Mainly used for testing purposes.
class file_input_stream : public finite_input_stream
{
public:

    /// Constructor
    file_input_stream();

    /// Constructor that opens the file immediately
    /// @throws std::system_error Thrown on failure.
    /// @param filename the filename
    file_input_stream(const std::string& filename);

    /// Opens the file
    /// @throws std::system_error Thrown on failure.
    /// @param filename the file name
    void open(const std::string& filename);

    /// Opens the file
    /// @param filename the file name
    /// @param ec on error set to indicate the type of error
    void open(const std::string& filename, std::error_code& ec);

    /// Closes the file
    void close();

public: // From finite_input_stream

    /// @copydoc finite_input_stream::seek()
    void seek(uint32_t pos);

    /// @copydoc finite_input_stream::read_position()
    uint32_t read_position();

    /// @copydoc finite_input_stream::size()
    uint32_t size();

public: // From input_stream

    /// @copydoc input_stream::read()
    void read(uint8_t* buffer, uint32_t bytes);

    /// @copydoc input_stream::bytes_available()
    uint32_t bytes_available();

private:

    /// The actual file
    std::ifstream m_file;

    /// The size of the file in bytes
    uint32_t m_filesize;
};
}
