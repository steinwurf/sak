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

        /// The allocate function
        using allocate_function = std::function<value_ptr()>;

        /// The recycle function
        using recycle_function = std::function<void(value_ptr)>;

    public:

        // Default constructor
        resource_pool() :
            m_pool(std::make_shared<impl>(
                       allocate_function(std::make_shared<value_type>)))
        { }

        resource_pool(allocate_function allocate) :
            m_pool(std::make_shared<impl>(std::move(allocate)))
        { }

        resource_pool(allocate_function allocate, recycle_function recycle) :
            m_pool(std::make_shared<impl>(std::move(allocate),
                                          std::move(recycle)))
        { }

        /// @returns the number of unused resources
        uint32_t unused_resources() const
        {
            assert(m_pool);
            return m_pool->free();
        }

        /// Frees all unused resources
        void free_unused()
        {
            assert(m_pool);
            m_pool->free_unused();
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

            impl(allocate_function allocate) :
                m_pool_size(0),
                m_allocate(std::move(allocate))
            {
                assert(m_allocate);
            }

            impl(allocate_function allocate, recycle_function recycle) :
                m_pool_size(0),
                m_allocate(std::move(allocate)),
                m_recycle(std::move(recycle))
            {
                assert(m_allocate);
                assert(m_recycle);
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
                    assert(m_allocate);
                    resource = m_allocate();
                    ++m_pool_size;
                }

                auto pool = impl::shared_from_this();

                return value_ptr(resource.get(), deleter(pool, resource));
            }

            void free_unused()
            {
                assert(m_pool_size >= m_free_list.size());

                m_pool_size -= m_free_list.size();
                m_free_list.clear();
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
                if (m_recycle)
                {
                    m_recycle(resource);
                }

                m_free_list.push_back(resource);
            }

        private:

            // The total number of resource allocated
            uint32_t m_pool_size;

            // The allocator to use
            allocate_function m_allocate;

            // The allocator to use
            recycle_function m_recycle;

            // Stores all the free resources
            std::list<value_ptr> m_free_list;

        };

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
