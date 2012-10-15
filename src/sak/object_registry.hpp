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

#ifndef SAK_OBJECT_REGISTRY_HPP
#define SAK_OBJECT_REGISTRY_HPP

#include <map>

#include "object_factory_impl.hpp"

namespace sak
{

    /// Object registry used to store factories to construct objects of
    /// the registered types.
    class object_registry
    {
    private:

        /// The map associating an object id to an object factory
        typedef std::map<object_id, boost::shared_ptr<object_factory> >
            object_map;

    private:

        /// Constructor of the factory registry
        object_registry()
            { }

    public:

        /// The factory registry is a singleton which can be accessed through
        /// this function
        /// @return the factory instance
        static object_registry* instance()
            {
                static object_registry registery;
                return &registery;
            }


        /// @return a factory stored in the registry
        template<class Factory>
        boost::shared_ptr<Factory> get_factory()
            {
                auto factory_id = get_object_id<Factory>();
                auto factory = find_factory(m_lookup_by_factory_id, factory_id);

                assert(factory);

                typedef object_factory_impl<Factory> factory_type;

                auto factory_impl =
                    boost::dynamic_pointer_cast<factory_type>(factory);

                assert(factory_impl);
                assert(factory_impl->m_factory);
                return factory_impl->m_factory;
            }

        /// Registers an object factory with the object registry
        /// Once a factory has been registered objects can be created using
        /// the sak::create<object_type>() function.
        template<class Factory, class Object>
        void set_factory()
            {

                auto factory_id  = get_object_id<Factory>();
                auto object_id   = get_object_id<Object>();

                auto factory =
                    boost::make_shared< object_factory_impl<Factory> >();

                m_lookup_by_factory_id[factory_id] = factory;
                m_lookup_by_object_id[object_id] = factory;
            }

        /// Clears all registered factories
        void clear_factories()
            {
                m_lookup_by_factory_id.clear();
                m_lookup_by_object_id.clear();
            }

        /// @return an object created using the registered factories
        template<class Object>
        boost::shared_ptr<Object> create()
            {
                assert(has_object_id(m_lookup_by_object_id, *Object::id()));

                auto factory =
                    find_factory(m_lookup_by_object_id, *Object::id());

                assert(factory);

                auto object = factory->build();

                return boost::dynamic_pointer_cast<Object>(object);
            }

    private:

        /// Checks if an object id is registered in a specific category
        bool has_object_id(const object_map &map, const object_id &id) const
            {
                return map.find(id) != map.end();
            }

        template<class Object>
        object_id get_object_id() const
            {
                return *Object::id();
            }

        /// Finds and returns an object factory in the given map with a
        /// "compatible" object_id.
        boost::shared_ptr<object_factory>
        find_factory(const object_map &map, const object_id &id) const
            {
                assert(has_object_id(map,id));
                return map.at(id);
            }

    private:

        /// Map allowing a factory to be found based on an object's object id
        object_map m_lookup_by_object_id;

        /// Map allowing a factory to be found based on a factory's object id
        object_map m_lookup_by_factory_id;

    };

    /// @see object_registry::create()
    template<class T>
    inline boost::shared_ptr<T> create()
    {
        return object_registry::instance()->create<T>();
    }

    /// @see object_registry::set_factory()
    template<class Factory, class Object>
    inline void set_factory()
    {
        return object_registry::instance()->set_factory<Factory, Object>();
    }

    /// @see object_registry::clear_factories()
    inline void clear_factories()
    {
        return object_registry::instance()->clear_factories();
    }

    /// @see object_registry::get_factory()
    template<class T>
    inline boost::shared_ptr<T> get_factory()
    {
        return object_registry::instance()->get_factory<T>();
    }

}

#endif

