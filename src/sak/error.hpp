// Copyright (c) 2012 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

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
            const char* name() const noexcept;

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
