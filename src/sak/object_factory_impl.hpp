// Copyright (c) 2012 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cassert>
#include <memory>
#include <functional>

#include "object_factory.hpp"


namespace sak
{
    class object_registry;

    /// Object factory used to embed a concrete factory
    template<class Factory>
    struct object_factory_impl : public object_factory
    {

        object_factory_impl() :
            m_factory(std::make_shared<Factory>())
        {}

        /// @return an object created using the user provided
        ///         factory
        virtual std::shared_ptr<void> build(object_registry& registry)
        {
            assert(m_factory);

            return m_factory->build(registry);
        }

        /// The user provided factory
        std::shared_ptr<Factory> m_factory;

    };

    /// Object factory function struct stores a function object
    struct object_factory_function : public object_factory
    {

        typedef std::function < std::shared_ptr<void> (object_registry&) >
        factory_callback;

        object_factory_function(const factory_callback& function) :
            m_function(function)
        {}

        /// @return an object created using the user provided
        ///         factory
        virtual std::shared_ptr<void> build(object_registry& registry)
        {
            assert(m_function);

            return m_function(registry);
        }

        /// The user provided factory
        factory_callback m_function;

    };

}
