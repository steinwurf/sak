// Copyright (c) 2011-2012 Steinwurf ApS
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

#ifndef SAK_OBJECT_HPP
#define SAK_OBJECT_HPP

#include <cstdint>
#include <cstring>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include "object_id.hpp"

namespace sak
{

    /// Object aggregation facility allows objects to be merged, accessed from
    /// all other aggregated objects
    ///
    /// @see test_object_xyz.cpp for example use
    class object
    {
    public:

        /// per object information stored in a shared vector
        struct object_data
        {
            object_id *m_object_id;
            boost::shared_ptr<object> m_object;
        };

        /// the object vector
        typedef std::vector<object_data> object_vector;

        /// pointer to the object vector
        typedef boost::shared_ptr<object_vector> object_storage;

    public:

        /// Constructor
        object()
            {
                m_object_storage = boost::make_shared<object_vector>();
            }

        /// Destructor
        virtual ~object()
            {
                object_vector::iterator it;
                for(it = m_object_storage->begin();
                    it != m_object_storage->end(); ++it)
                {
                    it->m_object.reset();
               }
            }

        /// Must be implemented by all objects inheriting from object
        /// @return object id
        static object_id* id()
            {
                static object_id id = object_id(object::register_type())
                    .set_name("object");

                return &id;
            }

        /// @return a unique numerical identifier for types in an object
        static uint16_t register_type()
            {
                static uint16_t id = 1;
                return id++;
            }

        /// Adds an object to the shared object storage
        /// @param object pointer to the object being aggregated
        template<class T>
        void set_object(boost::shared_ptr<T> object)
            {
                boost::shared_ptr<T> o = get_object<T>();
                assert(o.get() == 0);

                m_object_storage->insert(m_object_storage->begin(),
                                         object->m_object_storage->begin(),
                                         object->m_object_storage->end());

                object_data d = { T::id(), object };
                m_object_storage->push_back(d);

                object->m_object_storage = m_object_storage;
            }

        /// Looks up the object of the specified type
        /// @returns pointer to the requested interface if not found returns 0
        template<class T>
        inline boost::shared_ptr<T> get_object()// const
            {
                // @todo: fix this wierd thing with the current variable
                object_id *current;
                object_id *id = T::id();
                object_id *object_id = object::id();

                object_vector::iterator it;
                for(it = m_object_storage->begin(); it != m_object_storage->end(); ++it)
                {
                    // if here it fails -> object_id *current;
                    current = it->m_object_id;

                    while(current->m_id != id->m_id && current->m_id != object_id->m_id)
                    {
                        current = current->m_parent;
                    }

                    if(current->m_id == id->m_id)
                    {
                        return boost::dynamic_pointer_cast<T>(it->m_object);
                    }
                }

                return boost::shared_ptr<T>();
            }

    private:

        // object storage
        object_storage m_object_storage;

    };




}

#endif

