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

#ifndef SAK_OBJECT_FACTORY_HPP
#define SAK_OBJECT_FACTORY_HPP

#include <cstdint>
#include <map>

namespace sak
{

    class object_factory
    {
    public:
        virtual boost::shared_ptr<object> build() = 0;
    };

    template<class Factory>
    class real_factory : public object_factory
    {
    public:

        virtual boost::shared_ptr<object> build()
            {
                return m_factory->build();
            }

        boost::shared_ptr<Factory> m_factory;
    };


    class factory_registry
    {

        factory_registry(){}

    public:

        static factory_registry* instance()
            {
                static factory_registry registery;
                return &registery;
            }

        template<class Factory>
        void set_factory()
            {
                typedef typename Factory::object_type object_type;

                auto object_id = *object_type::id();
                assert(m_factory_lookup.find(object_id) == m_factory_lookup.end());
                assert(m_factory_reverse_lookup.find(object_id) == m_factory_reverse_lookup.end());

                std::cout << "Register " << object_id.m_name << std::endl;

                auto factory = boost::make_shared< real_factory<Factory> >();
                factory->m_factory = boost::make_shared<Factory>();

                m_factory_lookup[object_id] = factory;
                m_factory_reverse_lookup[*Factory::id()] = factory;

                // m_factory_lookup[object_id] = factory_id;
                std::cout << "map size " << m_factory_lookup.size() << std::endl;
            }

        template<class Factory>
        boost::shared_ptr<Factory> get_factory()
            {
                auto id = *Factory::id();
                assert(m_factory_reverse_lookup.find(id) != m_factory_reverse_lookup.end());

                auto o = m_factory_reverse_lookup[id];
                auto f = boost::dynamic_pointer_cast<real_factory<Factory> >(o);

                return f->m_factory;


            }

        template<class Object>
        boost::shared_ptr<Object> create()
            {
                std::cout << "map size " << m_factory_lookup.size() << std::endl;

                auto id = *Object::id();
                assert(m_factory_lookup.find(id) != m_factory_lookup.end());

                auto o = m_factory_lookup[id]->build();

                return boost::dynamic_pointer_cast<Object>(o);

            }


        typedef boost::shared_ptr<object_factory> object_factory_ptr;

        std::map<object_id, object_factory_ptr> m_factory_lookup;
        std::map<object_id, object_factory_ptr> m_factory_reverse_lookup;
//        std::map<object_id, boost::shared_ptr<object>

    };


    template<class T>
    inline boost::shared_ptr<T> create()
    {
        return factory_registry::instance()->create<T>();
    }

    template<class T>
    inline boost::shared_ptr<T> get_factory()
    {
        return factory_registry::instance()->get_factory<T>();
    }

}


#define REGISTER_FACTORY(factoryclass)                                      \
    static struct __##factoryclass##_factory                                \
    {                                                                       \
        __##factoryclass##_factory()                                        \
        {                                                                   \
            sak::factory_registry::instance()->set_factory<factoryclass>(); \
        }                                                                   \
    } x_##factory_name##_RegClass ;                                         \



#endif

