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

#include "object_id.hpp"

#include <cstring>
#include <iostream>

namespace sak
{

    object_id::object_id()
        : m_id(0),
          m_parent(0)
    { }

    object_id::object_id(const char* id)
        : m_id(id),
          m_parent(0)
	{ }

    object_id::object_id(const object_id &id)
        : m_id(id.m_id),
          m_parent(id.m_parent)          
    { }

    object_id& object_id::set_parent(object_id *parent)
    {
        m_parent = parent;
        return *this;
    }

    /*object_id& object_id::set_name(const char *name)
    {
        m_name = name;
        return *this;
    }*/

    bool object_id::has_parent()
    {
        return m_parent != 0;
    }

    /*const char* object_id::name() const
    {
        return m_name;
    }*/

    bool are_related(const object_id *id_a, const object_id *id_b)
    {
        assert(id_a != 0);
        assert(id_b != 0);

        const object_id *current_a = id_a;
        const object_id *current_b = id_b;
        //const object_id *object_id = object::id();

        assert(current_a->m_id != 0);
        assert(current_b->m_id != 0);
        //assert(object_id->m_id > 0);

        while(current_a != 0)
        {
            while(current_b != 0)
            {
                if(current_a->m_id == current_b->m_id)
                    return true;

                current_b = current_b->m_parent;
            }

            current_a = current_a->m_parent;
            current_b = id_b;

            //assert(current_a != 0);
            //assert(current_b != 0);
        }

        return false;
    }

    bool operator<(const object_id &id_a, const object_id &id_b)
    {
        // First we check if the objects are related
        if(are_related(&id_a, &id_b))
            return false;

        // Now we check using the id and the name
        if(id_a.m_id < id_b.m_id)
            return true;
        if(id_a.m_id > id_b.m_id)
            return false;
        /*if(strcmp(id_a.m_name, id_b.m_name) < 0)
            return true;
        if(strcmp(id_a.m_name, id_b.m_name) < 0)
            return false;*/

        return false;
    }

}

