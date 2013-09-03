// Copyright (c) 2012, Steinwurf ApS
// All rights reserved.

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

#pragma once

#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

#include <list>

namespace sak
{
    template<class Resource>
    class resource_pool
    {
    public:

        /// the type managed
        typedef Resource value_type;

        /// the pointer to the resource
        typedef boost::shared_ptr<value_type> value_ptr;

        /// the allocator function
        typedef boost::function<value_ptr ()> allocator_type;

    public:

        /// Create a new resource pool
        /// @param allocator the allocator to be used by the pool
        resource_pool(const allocator_type& allocator)
        {
            m_pool.reset( new pool_impl(allocator) );
        }

        /// @returns the number of resource in use
        uint32_t size() const
        {
            return m_pool->size();
        }

        /// @returns the number of unused resources
        uint32_t free() const
        {
            return m_pool->free();
        }

        /// @returns a resource from the pool
        value_ptr allocate()
        {
            return m_pool->allocate();
        }

    private:

        struct pool_impl : public boost::enable_shared_from_this<pool_impl>
        {

            pool_impl(const allocator_type& allocator)
                : m_allocator(allocator),
                  m_pool_size(0)
            {
                assert(m_allocator);
            }

            value_ptr allocate()
            {
                value_ptr resource;

                if (m_free_list.size() > 0)
                {
                    resource = m_free_list.back();
                    m_free_list.pop_back();
                }
                else
                {
                    assert(m_allocator);
                    resource = m_allocator();
                    ++m_pool_size;
                }

                boost::shared_ptr<pool_impl> pool = pool_impl::shared_from_this();

                return value_ptr(resource.get(), deleter(pool, resource));
            }

            uint32_t size() const
            {
                return m_pool_size;
            }

            uint32_t free() const
            {
                return static_cast<uint32_t>(m_free_list.size());
            }

            void recycle(const value_ptr& resource)
            {
                m_free_list.push_back(resource);
            }

        private:

            // The free resource list type
            typedef std::list<value_ptr> resource_list;

            // Stores all the free resources
            resource_list m_free_list;

            // The allocator to use
            allocator_type m_allocator;

            // The total number of resource allocated
            uint32_t m_pool_size;
        };

        typedef boost::shared_ptr<pool_impl> pool_ptr;
        typedef boost::weak_ptr<pool_impl> pool_weak_ptr;


        struct deleter
        {
            deleter(const pool_weak_ptr& pool, const value_ptr& resource)
                : m_pool(pool),
                  m_resource(resource)
            {
                assert(!m_pool.expired());
                assert(m_resource);
            }

            void operator()(value_type*)
            {
                // Place the resource in the free list
                pool_ptr pool = m_pool.lock();

                if (pool)
                {
                    pool->recycle(m_resource);
                }
            }

            // Poiner to the pool needed for recycling
            pool_weak_ptr m_pool;

            // The resource object
            value_ptr m_resource;
        };

    private:

        // The pool impl
        boost::shared_ptr<pool_impl> m_pool;

    };
}
