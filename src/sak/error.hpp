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

#pragma once

#include <cstdint>

#include <boost/system/error_code.hpp>
#include <boost/system/system_error.hpp>
#include <boost/throw_exception.hpp>

namespace sak
{

    namespace error
    {
        /// Enumeration of different error codes, we use a bit of
        /// macro uglyness to makes this easy (@see error.cpp to see
        /// how we make the error strings)
        enum error_type
        {
#define     ERROR_TAG(id,msg) id,
#include    "error_tags.hpp"
#undef      ERROR_TAG
            terminate_tag
        };


        /// sak errors
        /// @todo: replace with C++0x error handling - should be almost
        /// 100% compatible, i.e. simply replace the name-spaces.
        class sak_category_impl : public boost::system::error_category
        {
        public: // From boost::system::error_category

            /// @see boost::system::error_category::name()
            const char* name() const;

            /// @see boost::system::error_category::message()
            std::string message(int /*ev*/) const;
        };

        /// @return the beem error category
        const boost::system::error_category& sak_category();

        /// @return an error_code with beem errors
        boost::system::error_code make_error_code(error_type t);

        /// Throws an exception if the error code represents an error
        /// i.e. contains a non-zero error value.
        ///
        /// Example:
        ///
        /// boost::system::error_code ec =
        ///     error::make_error_code(error::invalid_ipv4);
        ///
        /// error::throw_error(ec);
        ///
        /// @param err an error code
        inline void throw_error(const boost::system::error_code& err)
        {
            if (err)
            {
                boost::system::system_error e(err);
                boost::throw_exception(e);
            }
        }
    }
}

/// Ensure that we can compare sak errors to reported error codes
namespace boost
{
    namespace system
    {

        template<>
        struct is_error_code_enum<sak::error::error_type>
        {
            static const bool value = true;
        };
    }
}
