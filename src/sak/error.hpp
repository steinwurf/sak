// Copyright (c) 2012 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>
#include <string>
#include <system_error>

namespace sak
{
    namespace error
    {
        /// Enumeration of different error codes
        enum error_type
        {
            failed_open_file = 1
        };

        /// sak error category with C++11 error handling
        class sak_category_impl : public std::error_category
        {
        public:

            const char* name() const throw();

            std::string message(int ev) const;
        };

        /// @return the sak error category
        const std::error_category& sak_category();

        /// @return an error_code with sak errors
        std::error_code make_error_code(error_type t);

        /// Throws an exception if the error code represents an error,
        /// i.e. contains a non-zero error value.
        ///
        /// Example:
        ///
        /// std::error_code ec = sak::error::failed_open_file;
        ///
        /// sak::error::throw_error(ec);
        ///
        /// @param err an error code
        inline void throw_error(const std::error_code& ec)
        {
            if (ec)
            {
                std::system_error exception(ec);
                throw exception;
            }
        }
    }
}

// Ensure that we can create sak errors from the declared error codes
namespace std
{
    template<>
    struct is_error_code_enum<sak::error::error_type> : public true_type
    { };
}
