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
#include <type_traits>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include "object_factory_impl.hpp"

namespace sak
{
    /// Type Info template for specifying the base class
    /// Template must be specialized for all classes that have a base class
	///
    /// Use the following macro to specify the base class:
    ///     SAF_DEFINE_PARENT(myClass, myBaseClass)
	/// Important: This macro must be used in the global namespace!
	/// Note that multiple inheritance is not supported.
    template<class T>
    struct sak_type_info
    {
        typedef void Base;                
    };

#define SAK_DEFINE_PARENT(DERIVED_CLASS, BASE_CLASS) \
	namespace sak \
	{ \
		template<> \
		struct sak_type_info<DERIVED_CLASS> \
		{ \
			typedef BASE_CLASS Base; \
		}; \
	}
       

    /// Object registry used to store factories to construct objects of
    /// the registered types.
    class object_registry
    {
    private:
        /// The object_id that is used to index the registered types 
        typedef size_t object_id;

        /// The map associating an object id to an object factory
        typedef std::map<object_id, boost::shared_ptr<object_factory> >
            factory_map;

        /// The map associating an object id to an object factory
        typedef std::map<object_id, boost::shared_ptr<void> >
            object_map;

    public:

        /// Constructor of the factory registry
        object_registry()
        { }

        /// @return a factory stored in the registry
        template<class Factory>
        boost::shared_ptr<Factory> get_factory()
        {
            auto factory_id = get_object_id<Factory>();
            auto factory = find(m_lookup_by_factory_id, factory_id);

            assert(factory);

            typedef object_factory_impl<Factory> factory_type;

            auto factory_impl =
                boost::dynamic_pointer_cast<factory_type>(factory);

            assert(factory_impl);
            assert(factory_impl->m_factory);
            return factory_impl->m_factory;
        }

        /// Registers an object factory with the object registry
        /// Once a factory has been registered objects can be created
        template<class Factory, class Object>
        void set_factory()
        {
            auto factory_id = get_object_id<Factory>();
            auto object_id  = get_object_id<Object>();           

            auto factory =
                boost::make_shared< object_factory_impl<Factory> >();

            m_lookup_by_factory_id[factory_id] = factory;                           
            m_lookup_by_object_id[object_id] = factory;

            // If the base class is not void, 
            // then reuse the factory instance for the Base type
            typedef typename sak_type_info<Object>::Base Base;
            if (std::is_void< Base >::value == false)
                set_factory_instance< Base >(factory);
        }

		/// Registers an object factory instance with the object registry
        /// Once a factory has been registered objects can be created
        template<class Object>
        void set_factory_instance(const boost::shared_ptr<object_factory>& factory)
        {           
            auto object_id  = get_object_id<Object>();

            m_lookup_by_object_id[object_id] = factory;

            // If the base class is not void, 
            // then reuse the factory instance for the Base type
            typedef typename sak_type_info<Object>::Base Base;
			if (std::is_void< Base >::value == false)
                set_factory_instance< Base >(factory);            
        }

        /// Registers an object factory function with the object registry
        /// Once a factory function has been registered objects can be created
        template<class Object>
        void set_factory(const boost::function< 
			boost::shared_ptr<Object>(object_registry &)> & func)
        {
            auto object_id = get_object_id<Object>();            

            auto factory =
                boost::make_shared< object_factory_function<Object> >(func);

            m_lookup_by_object_id[object_id] = factory;

			// If the base class is not void, 
            // then reuse the factory instance for the Base type
            typedef typename sak_type_info<Object>::Base Base;
			if (std::is_void< Base >::value == false)
                set_factory_instance< Base >(factory);            
        }

        /// Registers an object factory with the object registry. This methods
        /// can be used for factories with a non-standard build function.
        /// Users can call the get function and invoke the custom build function
        template<class Object>
        void set_object()
        {
            auto object_id = get_object_id<Object>();            

            auto object = boost::make_shared<Object>();

            m_lookup_by_shared_object_id[object_id] = object;

			// If the base class is not void, 
            // then also register the Base type
            typedef typename sak_type_info<Object>::Base Base;
			if (std::is_void< Base >::value == false)
                set_object_instance< Base >(object);
        }

		template<class Object>
        void set_object_instance(const boost::shared_ptr<Object>& object)
        {
            auto object_id = get_object_id<Object>();           

            m_lookup_by_shared_object_id[object_id] = object;

			// If the base class is not void, 
            // then also register the Base type
            typedef typename sak_type_info<Object>::Base Base;
			if (std::is_void< Base >::value == false)
                set_object_instance< Base >(object);
        }

        /// @return a factory stored in the registry
        template<class Object>
        boost::shared_ptr<Object> get_object()
        {
            auto object_id = get_object_id<Object>();
            auto object = find(m_lookup_by_shared_object_id, object_id);

            assert(object);
            return boost::static_pointer_cast<Object>(object);
        }

        /// Clears all registered factories
        void clear_factories()
        {
            m_lookup_by_factory_id.clear();
            m_lookup_by_object_id.clear();
        }

        /// @return an object created using the registered factories
        template<class Object>
        boost::shared_ptr<Object> build()
        {
            // Have you forgotten to register the parent class for Object?
            assert(has_object_id(m_lookup_by_object_id, get_object_id<Object>() ));

            auto factory = find(m_lookup_by_object_id, get_object_id<Object>() );

            assert(factory);

            auto obj = factory->build(*this);

            return boost::static_pointer_cast<Object>(obj);
        }

    private:

        /// Checks if an object id is registered in a specific category
        template<class Map>
        bool has_object_id(const Map &map, const object_id &id) const
        {
            return map.find(id) != map.end();
        }

        template<class Object>
        object_id get_object_id() const
        {
            return typeid(Object).hash_code();
        }

        /// Finds and returns an object factory in the given map with a
        /// "compatible" object_id.
        boost::shared_ptr<object_factory>
            find(const factory_map &map, const object_id &id) const
        {
            assert(has_object_id(map,id));
            return map.at(id);
        }


        /// Finds and returns an object factory in the given map with a
        /// "compatible" object_id.
        boost::shared_ptr<void>
            find(const object_map &map, const object_id &id) const
        {
            assert(has_object_id(map,id));
            return map.at(id);
        }

    private:

        /// Map allowing a factory to be found based on an object's object id
        factory_map m_lookup_by_object_id;

        /// Map allowing a shared object to be found based on an object's object id
        object_map m_lookup_by_shared_object_id;

        /// Map allowing a factory to be found based on a factory's object id
        factory_map m_lookup_by_factory_id;

    };
}

#endif

