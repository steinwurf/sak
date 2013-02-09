// Copyright (c) 2011-2012 Steinwurf ApS
// All Rights Reserved

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

#include <gtest/gtest.h>
#include <sak/object.hpp>
#include <sak/object_registry.hpp>

#include <boost/bind.hpp>

//#include "test_object_xyz_lib/test_object_xyz_lib_b.hpp"


class socket
{
public:
    virtual void write() = 0;
};


class rate_socket : public socket
{
public:
    void write()
	{
	}
};

template<>
sak::object_id* sak::get_object_id<rate_socket>()
{
    using namespace sak;
    static object_id id = object_id(typeid(rate_socket).name())
				.set_parent(get_object_id<socket>());
                
    return &id;
}


class rate_control
{
public:   

    void check_rate(boost::shared_ptr<socket> s)
	{
		//boost::shared_ptr<socket> s = get_object<socket>();

		assert(s);
		s->write();
	}

};

class rate_socket_factory
{
public:

    typedef rate_socket object_type;

    boost::shared_ptr<rate_socket> build(sak::object_registry &)
	{
		return boost::make_shared<rate_socket>();
	}
};


TEST(ObjectFactory, register_type)
{
    sak::object_registry registry;
    registry.set_factory<rate_socket_factory, rate_socket>();

    auto s = registry.build<socket>();

    auto f = registry.get_factory<rate_socket_factory>();

    s = f->build(registry);
}

boost::shared_ptr<rate_socket> build_rate_socket(sak::object_registry &)
{
    return boost::make_shared<rate_socket>();
}


TEST(ObjectFactory, register_type_function)
{
    sak::object_registry registry;
    registry.set_factory<rate_socket>(
        boost::bind(build_rate_socket, _1));

    auto s = registry.build<socket>();
}


/*TEST(ObjectFactory, register_type_from_lib)
{
    sak::object_registry registry;
    registry.set_factory<pear_factory, pear>();
    registry.set_factory<duck_factory, duck>();

    auto a = registry.build<duck>();

    EXPECT_EQ("duck eats fruit which is green", a->eat());

    registry.set_factory<apple_factory, apple>();
    a = registry.build<duck>();

    EXPECT_EQ("duck eats fruit which is red", a->eat());
}

TEST(ObjectFactory, get_factory)
{
    sak::object_registry registry;
    registry.set_factory<pear_factory, pear>();
    registry.set_factory<duck_factory, duck>();

    auto factory = registry.get_factory<duck_factory>();
    auto d = factory->build(registry);
    EXPECT_EQ("duck eats fruit which is green", d->eat());
}

TEST(ObjectFactory, set_get_object)
{
    sak::object_registry registry;
    registry.set_object<rate_socket>();

    auto rate_socket_1 = registry.get_object<rate_socket>();
    auto rate_socket_2 = registry.get_object<rate_socket>();

    EXPECT_TRUE(rate_socket_1);
    EXPECT_TRUE(rate_socket_2);
    EXPECT_EQ(rate_socket_1, rate_socket_2);
}*/


