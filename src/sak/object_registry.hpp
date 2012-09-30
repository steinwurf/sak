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

        /// The map associating a category id to an object map
        typedef std::map<uint32_t, object_map> category_map;

    private:

        /// Constructor of the factory registry
        object_registry()
            : m_current_category(object_category::default_id())
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

        /// Sets the current category - this determines precedence
        /// in how we find factories
        /// @param category_id the id of the category
        void set_category(uint32_t category_id)
            {
                m_current_category = category_id;
            }

        /// Returns the current category
        /// @return the category id of the current category
        uint32_t get_category() const
            {
                return m_current_category;
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

                return factory_impl->m_factory;
            }

        /// Registers an object factory with the object registry
        /// Once a factory has been registered objects can be created using
        /// the sak::create<object_type>() function. Factory registration
        /// should be done using the REGISTER_FACTORY(...) macro,
        template<class Factory, class Object>
        void set_factory()
            {

                auto factory_id  = get_object_id<Factory>();
                auto object_id   = get_object_id<Object>();

                // The object and factory should have the same category id
                assert(factory_id.m_category_id == object_id.m_category_id);

                assert(!has_in_category(m_lookup_by_factory_id, factory_id));
                assert(!has_in_category(m_lookup_by_object_id, object_id));

                auto factory = boost::make_shared< object_factory_impl<Factory> >();
                auto category_id = get_category_id<Factory>();

                m_lookup_by_factory_id[category_id][factory_id] = factory;
                m_lookup_by_object_id[category_id][object_id] = factory;
            }

        /// @return an object created using the registered factories
        template<class Object>
        boost::shared_ptr<Object> create()
            {
                auto factory = find_factory(m_lookup_by_object_id, *Object::id());
                assert(factory);

                auto object = factory->build();

                return boost::dynamic_pointer_cast<Object>(object);
            }

    private:

        /// Checks if an object id is registered in a specific category
        bool has_in_category(const category_map &map, const object_id &id, uint32_t category_id) const
            {
                if(map.find(category_id) != map.end())
                    return map.at(category_id).find(id) != map.at(category_id).end();
                else
                    return false;
            }

        /// Checks if an object id is registered in a specific category
        bool has_in_category(const category_map &map, const object_id &id) const
            {
                return has_in_category(map, id, id.m_category_id);
            }

        template<class Object>
        object_id get_object_id() const
            {
                return *Object::id();
            }

        /// @return the category id of the factory
        template<class Object>
        uint32_t get_category_id() const
            {
                return Object::id()->m_category_id;
            }

        /// Finds and returns an object factory in the given map with a
        /// "compatible" object_id.
        boost::shared_ptr<object_factory>
        find_factory(const category_map &map, const object_id &id) const
            {
                // First we look:
                // 1. In the category defined by the object
                // 2. In the current category
                // 3. In the default category
                if(has_in_category(map, id))
                {
                    return map.at(id.m_category_id).at(id);
                }
                else if(has_in_category(map, id, m_current_category))
                {
                    return map.at(m_current_category).at(id);
                }
                else if(has_in_category(map, id, object_category::default_id()))
                {
                    return map.at(object_category::default_id()).at(id);
                }

                return boost::shared_ptr<object_factory>();

            }

    private:

        /// The current category
        uint32_t m_current_category;

        /// Map allowing a factory to be found based on an objects
        /// object_id
        std::map<uint32_t, object_map> m_lookup_by_object_id;

        /// Map allowing a factory to be found based on a factory's
        /// object id
        std::map<uint32_t, object_map> m_lookup_by_factory_id;

    };

    /// @return an object created by a factory stored in the registry
    template<class T>
    inline boost::shared_ptr<T> create()
    {
        return object_registry::instance()->create<T>();
    }

    /// @return a factory stored in the object registry
    template<class T>
    inline boost::shared_ptr<T> get_factory()
    {
        return object_registry::instance()->get_factory<T>();
    }

    /// @param category which should be used by the object registry
    inline void set_factory_category(uint32_t category)
    {
        object_registry::instance()->set_category(category);
    }

}

/// Macro for registering a factory with the object registry
#define REGISTER_FACTORY(factoryclass, objectclass)                         \
    static struct __##factoryclass##_factory                                \
    {                                                                       \
        __##factoryclass##_factory()                                        \
        {                                                                   \
            sak::object_registry::instance()                                \
                ->set_factory<factoryclass, objectclass>();                 \
        }                                                                   \
    } x_##factoryclass##_RegClass ;                                         \

#endif

