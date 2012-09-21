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

class sender : public sak::object
{
public:

    static sak::object_id* id()
        {
            using namespace sak;
            static object_id id = object_id(object::register_type())
                .set_parent(object::id())
                .set_name("sender");

            return &id;
        }
};


class socket : public sak::object
{
public:

    static sak::object_id* id()
        {
            using namespace sak;
            static object_id id = object_id(object::register_type())
                .set_parent(object::id())
                .set_name("socket");

            return &id;
        }

    virtual void write() = 0;

};

class rate_socket : public socket
{
public:

    static sak::object_id* id()
        {
            using namespace sak;
            static object_id id = object_id(object::register_type())
                .set_parent(socket::id())
                .set_name("rate_socket");

            return &id;
        }

    void write()
        {
        }
};


class rate_control : public sak::object
{
public:

    static sak::object_id* id()
        {
            using namespace sak;
            static object_id id = object_id(object::register_type())
                .set_parent(object::id())
                .set_name("rate_control");

            return &id;
        }

    void check_rate()
        {
            boost::shared_ptr<socket> s = get_object<socket>();

            assert(s);
            s->write();
        }

};

TEST(Object, basic)
{
    boost::shared_ptr<sender> s
        = boost::make_shared<sender>();

    boost::shared_ptr<socket> ss
        = boost::make_shared<rate_socket>();

    boost::shared_ptr<rate_control> rate
        = boost::make_shared<rate_control>();

    s->set_object(ss);
    ss->set_object(rate);

    {
        boost::shared_ptr<socket> sock
            = s->get_object<socket>();

        sock->write();

        boost::shared_ptr<rate_control> r
            = s->get_object<rate_control>();

        r->check_rate();
    }

    rate->check_rate();
}

class a : public sak::object
{
public:

    static sak::object_id* id()
        {
            using namespace sak;
            static object_id id = object_id(object::register_type())
                .set_parent(object::id())
                .set_name("a");

            return &id;
        }

    a(uint32_t &counter)
        :m_counter(counter)
        {
            ++m_counter;
        }

    ~a()
        {
            --m_counter;
        }

    uint32_t &m_counter;
};

class b : public sak::object
{
public:

    static sak::object_id* id()
        {
            using namespace sak;
            static object_id id = object_id(object::register_type())
                .set_parent(object::id())
                .set_name("b");

            return &id;
        }

    b(uint32_t &counter)
        :m_counter(counter)
        {
            ++m_counter;
        }

    ~b()
        {
            --m_counter;
        }

    uint32_t &m_counter;
};

class c : public sak::object
{
public:

    static sak::object_id* id()
        {
            using namespace sak;
            static object_id id = object_id(object::register_type())
                .set_parent(object::id())
                .set_name("c");

            return &id;
        }

    c(uint32_t &counter)
        :m_counter(counter)
        {
            ++m_counter;
        }

    ~c()
        {
            --m_counter;
        }

    uint32_t &m_counter;
};

/// Test that all objects are correctly deleted
TEST(Object, destructor)
{
    uint32_t counter;

    {
        boost::shared_ptr<a> a1
            = boost::make_shared<a>(counter);

        boost::shared_ptr<b> b1
            = boost::make_shared<b>(counter);

        boost::shared_ptr<c> c1
            = boost::make_shared<c>(counter);

        EXPECT_EQ(counter, 3U);

        a1->set_object(b1);
        b1->set_object(c1);

        EXPECT_EQ(counter, 3U);
    }

    EXPECT_EQ(counter, 0U);
}

// Test that all objects can be retrieved from all objects, also the original
//object from any child object
//
// This test currently fails, find a solution to the problem
//
// TEST(Object, symmetric_access)
// {
//     boost::shared_ptr<sender> a
//         = boost::make_shared<sender>();

//     boost::shared_ptr<socket> b
//         = boost::make_shared<rate_socket>();

//     a->set_object(b);

//     boost::shared_ptr<socket> b2 = a->get_object<rate_socket>();
//     boost::shared_ptr<sender> a2 = b->get_object<sender>();

//     EXPECT_TRUE(a2.get() != 0);
// }

