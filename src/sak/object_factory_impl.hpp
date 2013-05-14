// Copyright (c) 2012 Steinwurf ApS
// All Rights Reserved
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

#pragma once

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include "object_factory.hpp"


namespace sak
{
    class object_registry;

    /// Object factory used to embed a concrete factory
    template<class Factory>
    struct object_factory_impl : public object_factory
    {

        object_factory_impl() :
            m_factory(boost::make_shared<Factory>())
        {}

        /// @return an object created using the user provided
        ///         factory
        virtual boost::shared_ptr<void> build(object_registry& registry)
        {
            assert(m_factory);

            return m_factory->build(registry);
        }

        /// The user provided factory
        boost::shared_ptr<Factory> m_factory;

    };

    /// Object factory function struct stores a function object
    struct object_factory_function : public object_factory
    {

        typedef boost::function < boost::shared_ptr<void> (object_registry&) >
        factory_callback;

        object_factory_function(const factory_callback& function) :
            m_function(function)
        {}

        /// @return an object created using the user provided
        ///         factory
        virtual boost::shared_ptr<void> build(object_registry& registry)
        {
            assert(m_function);

            return m_function(registry);
        }

        /// The user provided factory
        factory_callback m_function;

    };

}
