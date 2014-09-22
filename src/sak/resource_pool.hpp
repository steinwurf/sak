// Copyright (c) 2012 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <functional>
#include <memory>
#include <list>

namespace sak
{
    /// @brief The resource pool stores value objects and recycles them
    template<class Value>
    class resource_pool
    {
    public:

        /// The type managed
        using value_type = Value;

        /// The pointer to the resource
        using value_ptr = std::shared_ptr<value_type>;

        /// The allocator function
        using allocator_function = std::function<value_ptr()>;

        /// The recycle function
        using recycle_function = std::function<void(value_ptr)>;

    public:

        /// Default constructor
        resource_pool() :
            m_pool(std::make_shared<impl>())
        { }

        void set_allocator(const allocator_function& allocator)
        {
            assert(m_pool);
            m_pool->set_allocator(allocator);
        }

        void set_recycle_callback(const recycle_function& allocator)
        {
            assert(m_pool);
            assert(0);
            // m_pool->set_allocator(allocator);
        }

        /// @returns the number of resource in use
        uint32_t total_resources() const
        {
            assert(m_pool);
            return m_pool->size();
        }

        /// @returns the number of resource in use
        uint32_t used_resources() const
        {
            assert(m_pool);
            return m_pool->size();
        }

        /// @returns the number of unused resources
        uint32_t unused_resources() const
        {
            assert(m_pool);
            return m_pool->free();
        }

        /// @returns a resource from the pool
        value_ptr allocate()
        {
            assert(m_pool);
            return m_pool->allocate();
        }

    private:

        // The
        struct impl : public std::enable_shared_from_this<impl>
        {

            impl()
                : m_allocator(std::make_shared<value_type>),
                  m_pool_size(0)
            {
                assert(m_allocator);
            }

            void set_allocator(const allocator_function& allocator)
            {
                assert(allocator);
                m_allocator = allocator;
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

                auto pool = impl::shared_from_this();

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

            // Stores all the free resources
            std::list<value_ptr> m_free_list;

            // The allocator to use
            allocator_function m_allocator;

            // The total number of resource allocated
            uint32_t m_pool_size;
        };

        typedef std::shared_ptr<impl> pool_ptr;
        typedef std::weak_ptr<impl> pool_weak_ptr;


        struct deleter
        {
            deleter(const std::weak_ptr<impl>& pool,
                    const value_ptr& resource)
                : m_pool(pool),
                  m_resource(resource)
            {
                assert(!m_pool.expired());
                assert(m_resource);
            }

            void operator()(value_type*)
            {
                // Place the resource in the free list
                auto pool = m_pool.lock();

                if (pool)
                {
                    pool->recycle(m_resource);
                }
            }

            // Poiner to the pool needed for recycling
            std::weak_ptr<impl> m_pool;

            // The resource object
            value_ptr m_resource;
        };

    private:

        // The pool impl
        std::shared_ptr<impl> m_pool;

    };
}
