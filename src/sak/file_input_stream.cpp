// Copyright (c) 2012 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include "file_input_stream.hpp"

#include <cassert>
#include <fstream>

#include "error.hpp"

namespace sak
{
    file_input_stream::file_input_stream() :
        m_filesize(0)
    { }

    file_input_stream::file_input_stream(const std::string& filename) :
        m_filesize(0)
    {
        open(filename);
    }

    void file_input_stream::open(const std::string& filename)
    {
        assert(!m_file.is_open());

        boost::system::error_code ec;
        open(filename, ec);

        // If an error occurs, throw that
        if (ec)
        {
            error::throw_error(ec);
        }
    }

    void file_input_stream::open(const std::string& filename,
                                 boost::system::error_code& ec)
    {
        assert(!m_file.is_open());

        m_file.open(filename.c_str(),
                    std::ios::in | std::ios::binary);

        if (!m_file.is_open())
        {
            ec = error::make_error_code(error::failed_open_file);
            return;
        }

        m_file.seekg(0, std::ios::end);
        assert(m_file);

        // We cannot use the read_position function here due to a
        // problem on the iOS platform described in the read_position
        // function.
        auto pos = m_file.tellg();
        assert(pos >= 0);

        m_filesize = (uint32_t) pos;

        m_file.seekg(0, std::ios::beg);
        assert(m_file);
    }

    void file_input_stream::close()
    {
        assert(m_file.is_open());
        m_file.close();
    }

    void file_input_stream::seek(uint32_t pos)
    {
        assert(m_file.is_open());
        m_file.seekg(pos, std::ios::beg);
        assert(m_file);
    }

    uint32_t file_input_stream::read_position()
    {
        assert(m_file.is_open());

        // Workaround for problem on iOS where tellg returned -1 when
        // reading the last byte. However the EOF flag was correctly
        // set. So here we check for EOF, if it is true we set the
        // read_position = m_file_size

        if (m_file.eof())
        {
            return m_filesize;
        }
        else
        {
            std::streamoff pos = m_file.tellg();
            assert(pos >= 0);

            return static_cast<uint32_t>(pos);
        }
    }

    void file_input_stream::read(uint8_t* buffer, uint32_t bytes)
    {
        assert(m_file.is_open());
        m_file.read(reinterpret_cast<char*>(buffer), bytes);

        assert(bytes == static_cast<uint32_t>(m_file.gcount()));
    }

    uint32_t file_input_stream::bytes_available()
    {
        assert(m_file.is_open());
        uint32_t pos = read_position();
        assert(pos <= m_filesize);

        return m_filesize - pos;
    }

    uint32_t file_input_stream::size()
    {
        assert(m_file.is_open());
        return m_filesize;
    }
}
