// Copyright (c) 2012 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <memory>

namespace sak
{
    class object_registry;

    /// Base class for an object factory
    class object_factory
    {
    public:
        /// @return an object of the specified type
        virtual std::shared_ptr<void> build(object_registry&) = 0;
    };

}
