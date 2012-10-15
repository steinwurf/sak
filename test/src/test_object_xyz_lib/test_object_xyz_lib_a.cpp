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

#include "test_object_xyz_lib_a.hpp"
#include <sak/object_registry.hpp>

//
// Fruit
//

sak::object_id* fruit::id()
{
    using namespace sak;
    static object_id id = object_id(object::register_type())
        .set_parent(object::id())
        .set_name("fruit");

    return &id;
}

//
// Apple
//

sak::object_id* apple::id()
{
    using namespace sak;
    static object_id id = object_id(object::register_type())
        .set_parent(fruit::id())
        .set_name("apple");

    return &id;
}

std::string apple::color()
{
    return "red";
}

//
// Apple Factory
//

sak::object_id* apple_factory::id()
{
    using namespace sak;
    static object_id id = object_id(object::register_type())
        .set_parent(object::id())
        .set_name("apple_factory");

    return &id;
}

boost::shared_ptr<apple> apple_factory::build()
{
    return boost::make_shared<apple>();
}


//
// Pear
//

sak::object_id* pear::id()
{
    using namespace sak;
    static object_id id = object_id(object::register_type())
        .set_parent(fruit::id())
        .set_name("pear");

    return &id;
}

std::string pear::color()
{
    return "green";
}

//
// Pear Factory
//

sak::object_id* pear_factory::id()
{
    using namespace sak;
    static object_id id = object_id(object::register_type())
        .set_parent(object::id())
        .set_name("pear_factory");

    return &id;
}

boost::shared_ptr<pear> pear_factory::build()
{
    return boost::make_shared<pear>();
}












