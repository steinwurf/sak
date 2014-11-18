// Copyright (c) 2012 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include "error.hpp"

namespace sak
{
    namespace error
    {
        const char* sak_category_impl::name() const
        {
            return "sak";
        }

        std::string sak_category_impl::message(int ev) const
        {
            switch (ev)
            {
            case error_type::failed_open_file:
                return "Failed to open file";
            default:
                return "Unknown error";
            }
        }
        const std::error_category& sak_category()
        {
            static sak_category_impl instance;
            return instance;
        }

        std::error_code make_error_code(error_type t)
        {
            return std::error_code(static_cast<int>(t), sak_category());
        }
    }
}
