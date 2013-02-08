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

#ifndef SAK_OBJECT_ID_HPP
#define SAK_OBJECT_ID_HPP

#include <cstdint>

namespace sak
{
    /// Object id used to identify object types in the object
    /// aggregation and object factory systems.
    struct object_id
    {
    public:

        /// Construct a new invalid object_id
        object_id();

        /// Construct a new object id with the specified id, the
        /// id should be registered by calling object::register_type()
        object_id(uint16_t id);

        /// Construct an object id from another object id
        object_id(const object_id &id);

        /// Set the parent of the object - should be specified if
        /// inheritance is used.
        object_id& set_parent(object_id *parent);

        /// Set the name of the object this should correspond to the
        /// class/type of the object
        object_id& set_name(const char *name);

        /// @return true if the object_id has a parent specified
        bool has_parent();

        /// @return the name of the object
        const char* name() const;

    public:

        /// The id of the object
        uint16_t m_id;

        /// Pointer to the parent object
        object_id *m_parent;

        /// The name of the objects type/class.
        const char* m_name;
    };

    /// @return true if the two objects are related by inheritance
    bool are_related(const object_id *id_a, const object_id *id_b);

    /// @return true if the objects are considered less than returns
    ///         false if the object is larger or equal
    bool operator<(const object_id &id_a, const object_id &id_b);
}



#endif

