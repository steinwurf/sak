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
    /// @brief The resource pool stores value objects and recycles them.
    ///
    /// The resource pool

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

        /// Create a resource
        resource_pool(allocate_function allocate) :
            m_pool(std::make_shared<impl>(std::move(allocate)))
        { }

        resource_pool(allocate_function allocate, recycle_function recycle) :
            m_pool(std::make_shared<impl>(std::move(allocate),
                                          std::move(recycle)))
        { }

        resource_pool(const resource_pool& other) :
            m_pool(std::make_shared<impl>(*other.m_pool))
        { }

        resource_pool(resource_pool&& other) :
            m_pool(std::move(other.m_pool))
        {
            assert(m_pool);
        }

        resource_pool& operator=(const resource_pool& other)
        {
            resource_pool tmp(other);
            std::swap(*this, tmp);
            return *this;
        }

        resource_pool& operator=(resource_pool&& other)
        {
            m_pool = std::move(other.m_pool);
            return *this;
        }

        /// @returns the number of unused resources
        uint32_t unused_resources() const
        {
            assert(m_pool);
            return m_pool->unused_resources();
        }

        /// Frees all unused resources
        void free_unused()
        {
            assert(m_pool);
            m_pool->free_unused();
        }

        /// @return A resource from the pool.
        value_ptr allocate()
        {
            assert(m_pool);
            return m_pool->allocate();
        }

    private:

        /// The actual pool implementation. We use the
        /// enable_shared_from_this helper to make sure we can pass a
        /// "back-pointer" to the pooled objects. The idea behind this
        /// is that we need objects to be able to add themselves back
        /// into the pool once they go out of scope.
        struct impl : public std::enable_shared_from_this<impl>
        {

            impl(allocate_function allocate) :
                m_allocate(std::move(allocate))
            {
                assert(m_allocate);
            }

            impl(allocate_function allocate, recycle_function recycle) :
                m_allocate(std::move(allocate)),
                m_recycle(std::move(recycle))
            {
                assert(m_allocate);
                assert(m_recycle);
            }

            impl(const impl& other) :
                std::enable_shared_from_this<impl>(other),
                m_allocate(other.m_allocate),
                m_recycle(other.m_recycle)
            {
                for (uint32_t i = 0; i < other.unused_resources(); ++i)
                {
                    m_free_list.push_back(m_allocate());
                }
            }

            impl(impl&& other) :
                std::enable_shared_from_this<impl>(other),
                m_allocate(std::move(other.m_allocate)),
                m_recycle(std::move(other.m_recycle)),
                m_free_list(std::move(other.m_free_list))
            { }

            impl& operator=(const impl& other)
            {
                impl tmp(other);
                std::swap(*this, tmp);
                return *this;
            }

            impl& operator=(impl&& other)
            {
                m_allocate = std::move(other.m_allocate);
                m_recycle = std::move(other.m_recycle);
                m_free_list = std::move(other.m_free_list);
                return *this;
            }

            /// Allocate a new value from the pool
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
                }

                auto pool = impl::shared_from_this();

                // Here we create a std::shared_ptr<T> with a naked
                // pointer to the resource and a custom deleter
                // object. The custom deleter object stores two
                // things:
                //
                //   1. A std::weak_ptr<T> to the pool (used when we
                //      need to put the resource back in the
                //      pool)
                //
                //   2. A std::shared_ptr<T> that points to the actual
                //      resource and is the one actually keeping it alive.

                return value_ptr(resource.get(), deleter(pool, resource));
            }

            void free_unused()
            {
                m_free_list.clear();
            }

            uint32_t unused_resources() const
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

            // The allocator to use
            allocate_function m_allocate;

            // The allocator to use
            recycle_function m_recycle;

            // Stores all the free resources
            std::list<value_ptr> m_free_list;

        };

        /// The delete object used by the std::shared_ptr<T> to
        /// de-allocate the object if the pool goes out of scope. When
        /// a std::hared_ptr wants to de-allocate the object contained
        /// it will call the operator() define here.
        struct deleter
        {
            /// @param pool. A weak_ptr to the pool
            deleter(const std::weak_ptr<impl>& pool,
                    const value_ptr& resource)
                : m_pool(pool),
                  m_resource(resource)
            {
                assert(!m_pool.expired());
                assert(m_resource);
            }

            /// Call operator called by std::shared_ptr<T> when
            /// de-allocating the object.
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
