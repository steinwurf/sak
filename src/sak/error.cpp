// Copyright (c) 2012 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include "error.hpp"

namespace sak
{

    namespace error
    {

        const char* error_tags[256] =
        {
#define     ERROR_TAG(id, msg) msg,
#include    "error_tags.hpp"
#undef      ERROR_TAG
        };

        const char* sak_category_impl::name() const
        {
            return "sak";
        }

        std::string sak_category_impl::message(int ev) const
        {
            return error_tags[ev];
        }

        const boost::system::error_category& sak_category()
        {
            static sak_category_impl instance;
            return instance;
        }

        boost::system::error_code make_error_code(error_type t)
        {
            return boost::system::error_code(static_cast<int>(t),
                                             sak_category());
        }

    }

}
