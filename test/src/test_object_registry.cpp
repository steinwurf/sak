// Copyright (c) 2011-2013 Steinwurf ApS
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

#include <gtest/gtest.h>
#include <boost/bind.hpp>

#include <sak/object_registry.hpp>
#include "test_object_xyz_lib/test_object_xyz_lib_b.hpp"

// ----- BASIC SOCKET TEST -------
class socket
{
public:

    virtual std::string write() = 0;
};

class rate_socket : public socket
{
public:

    std::string write()
    {
        return std::string("rate_socket write");
    }
};
SAK_DEFINE_PARENT(rate_socket, socket)

class rate_socket_factory
{
public:

    typedef rate_socket object_type;

    boost::shared_ptr<rate_socket> build(sak::object_registry&)
    {
        return boost::make_shared<rate_socket>();
    }
};

// Testing that the building a base type with the registry works
TEST(ObjectFactory, register_type)
{
    sak::object_registry registry;
    registry.set_factory<rate_socket_factory, rate_socket>();

    auto s = registry.build<socket>();
    EXPECT_EQ("rate_socket write", s->write());
    auto f = registry.get_factory<rate_socket_factory>();

    s = f->build(registry);
    EXPECT_EQ("rate_socket write", s->write());
}

boost::shared_ptr<rate_socket> build_rate_socket(sak::object_registry&)
{
    return boost::make_shared<rate_socket>();
}

// Testing factory function
TEST(ObjectFactory, register_type_function)
{
    sak::object_registry registry;
    registry.set_factory<rate_socket>(
        boost::bind(build_rate_socket, _1));

    auto s = registry.build<socket>();
    EXPECT_EQ("rate_socket write", s->write());
}

// Testing set_object + get_object
TEST(ObjectFactory, set_get_object)
{
    sak::object_registry registry;
    registry.set_object<rate_socket>();

    auto rate_socket_1 = registry.get_object<rate_socket>();
    auto rate_socket_2 = registry.get_object<socket>();

    EXPECT_TRUE(rate_socket_1 != 0);
    EXPECT_TRUE(rate_socket_2 != 0);
    EXPECT_EQ(rate_socket_1, rate_socket_2);
}

// ----- NAMESPACE TEST -------
namespace foobar
{
    class magic_socket : public rate_socket
    {
    public:

        std::string write()
        {
            return std::string("magic_socket write");
        }
    };

    class magic_socket_factory
    {
    public:

        typedef rate_socket object_type;

        boost::shared_ptr<magic_socket> build(sak::object_registry&)
        {
            return boost::make_shared<magic_socket>();
        }
    };
}
SAK_DEFINE_PARENT(foobar::magic_socket, rate_socket)

// Testing parent definition inside other namespace
TEST(ObjectFactory, register_type_namespace)
{
    using namespace foobar;
    sak::object_registry registry;
    registry.set_factory<magic_socket_factory, magic_socket>();

    auto s = registry.build<socket>();
    EXPECT_EQ("magic_socket write", s->write());
}


// ----- FLOWER FACTORY TEST -------
enum Color { red, green, blue };

class plant
{
    Color m_color;

public:

    plant(Color c): m_color(c) {}
    Color color() { return m_color; }
};

class flower: public plant
{
public:

    flower(Color c): plant(c) {}
};

SAK_DEFINE_PARENT(flower, plant)

class flower_factory
{
    Color m_color;
public:

    flower_factory(): m_color(Color::green) {} // use green as default color

    void set_color(Color c) { m_color = c; }

    boost::shared_ptr<flower> build(sak::object_registry&)
    {
        return boost::make_shared<flower>(m_color);
    }
};

// Testing factory with custom parameters
TEST(ObjectFactory, set_factory_param)
{
    sak::object_registry registry;
    registry.set_factory<flower_factory, flower>();

    // Build a "default" flower
    auto fl = registry.build<plant>();
    EXPECT_EQ(Color::green, fl->color());

    // Get the factory and change the color to red
    auto factory = registry.get_factory<flower_factory>();
    factory->set_color(Color::red);

    // Build a red flower
    auto fl2 = registry.build<plant>();
    EXPECT_EQ(Color::red, fl2->color());
}

// Testing factory for types defined in a static library
TEST(ObjectFactory, register_type_from_lib)
{
    sak::object_registry registry;
    // Pear factory will produce fruits
    registry.set_factory<pear_factory, pear>();
    registry.set_factory<duck_factory, duck>();

    auto a = registry.build<duck>();

    // The duck should eat a green pear now
    EXPECT_EQ("duck eats fruit which is green", a->eat());

    // Change the factory that produces fruit
    registry.set_factory<apple_factory, apple>();
    a = registry.build<duck>();
    // The duck should eat a red apple now
    EXPECT_EQ("duck eats fruit which is red", a->eat());
}

// Testing set_factory + get_factory
TEST(ObjectFactory, get_factory)
{
    sak::object_registry registry;
    registry.set_factory<pear_factory, pear>();
    registry.set_factory<duck_factory, duck>();

    auto factory = registry.get_factory<duck_factory>();
    auto d = factory->build(registry);
    EXPECT_EQ("duck eats fruit which is green", d->eat());
}
