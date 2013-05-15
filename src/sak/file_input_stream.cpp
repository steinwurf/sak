// Copyright (c) 2012, Steinwurf ApS
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

#include "file_input_stream.hpp"
#include "error.hpp"

#include <fstream>
#include <cassert>

namespace sak
{

    file_input_stream::file_input_stream()
        : m_filesize(0)
    {
    }

    file_input_stream::file_input_stream(const std::string& filename)
        : m_filesize(0)
    {
        open(filename);
    }


    void file_input_stream::open(const std::string& filename)
    {
        assert(!m_file.is_open());

        boost::system::error_code ec;
        open(filename, ec);

        // If an error throw
        error::throw_error(ec);
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
        m_filesize = read_position();
        m_file.seekg(0, std::ios::beg);
    }

    void file_input_stream::seek(uint32_t pos)
    {
        assert(m_file.is_open());
        assert(m_file.seekg(pos, std::ios::beg));
    }

    uint32_t file_input_stream::read_position()
    {
        assert(m_file.is_open());

        std::streamoff pos = m_file.tellg();
        assert(pos >= 0);

        return static_cast<uint32_t>(pos);
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
