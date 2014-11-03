// Copyright (c) 2012 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <functional>
#include <list>
#include <memory>
#include <utility>

namespace sak
{
    /// @brief The resource pool stores value objects and recycles them.
    template<class Value>
    class resource_pool
    {
    public:

        /// The type managed
        using value_type = Value;

        /// The pointer to the resource
        using value_ptr = std::shared_ptr<value_type>;

        /// The allocate function type
        /// Should take no arguments and return an std::shared_ptr to the Value
        using allocate_function = std::function<value_ptr()>;

        /// The recycle function type
        /// If specified the recycle function will be called every time a
        /// resource gets recycled into the pool. This allows temporary
        /// resources, e.g., file handles to be closed when an object is longer
        /// used.
        using recycle_function = std::function<void(value_ptr)>;

    public:

        /// Default constructor, we only want this to be available
        /// i.e. the resource_pool to be default constructible if the
        /// value_type we build is default constructible.
        ///
        /// This means that we only want
        /// std::is_default_constructible<resource_pool<T>>::value to
        /// be true if the type T is default constructible.
        ///
        /// Unfortunately this does not work if we don't do the
        /// template magic seen below. What we do there is to use
        /// SFINAE to disable the default constructor for non default
        /// constructible types.
        ///
        /// It looks quite ugly and if somebody can fix in a simpler way
        /// please do :)
        template
        <
            class T = Value,
            typename std::enable_if<
                std::is_default_constructible<T>::value, uint8_t>::type = 0
        >
        resource_pool() :
            m_pool(std::make_shared<impl>(
                       allocate_function(std::make_shared<value_type>)))
        { }

        /// Create a resource pool using a specific allocate function.
        /// @param allocate Allocation function
        resource_pool(allocate_function allocate) :
            m_pool(std::make_shared<impl>(std::move(allocate)))
        { }

        /// Create a resource pool using a specific allocate function and
        /// recycle function.
        /// @param allocate Allocation function
        /// @param recycle Recycle function
        resource_pool(allocate_function allocate, recycle_function recycle) :
            m_pool(std::make_shared<impl>(std::move(allocate),
                                          std::move(recycle)))
        { }

        /// Copy constructor
        resource_pool(const resource_pool& other) :
            m_pool(std::make_shared<impl>(*other.m_pool))
        { }

        /// Move constructor
        resource_pool(resource_pool&& other) :
            m_pool(std::move(other.m_pool))
        {
            assert(m_pool);
        }

        /// Copy assignment
        resource_pool& operator=(const resource_pool& other)
        {
            resource_pool tmp(other);
            std::swap(*this, tmp);
            return *this;
        }

        /// Move assignment
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

            /// @copydoc resource_pool::resource_pool(allocate_function)
            impl(allocate_function allocate) :
                m_allocate(std::move(allocate))
            {
                assert(m_allocate);
            }

            /// @copydoc resource_pool::resource_pool(allocate_function,
            ///                                       recycle_function)
            impl(allocate_function allocate, recycle_function recycle) :
                m_allocate(std::move(allocate)),
                m_recycle(std::move(recycle))
            {
                assert(m_allocate);
                assert(m_recycle);
            }

            /// Copy constructor
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

            /// Move constructor
            impl(impl&& other) :
                std::enable_shared_from_this<impl>(other),
                m_allocate(std::move(other.m_allocate)),
                m_recycle(std::move(other.m_recycle)),
                m_free_list(std::move(other.m_free_list))
            { }

            /// Copy assignment
            impl& operator=(const impl& other)
            {
                impl tmp(other);
                std::swap(*this, tmp);
                return *this;
            }

            /// Move assignment
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
                //      need to put the resource back in the pool). If
                //      the pool dies before the resource then we can
                //      detect this with the weak_ptr and no try to
                //      access it.
                //
                //   2. A std::shared_ptr<T> that points to the actual
                //      resource and is the one actually keeping it alive.

                return value_ptr(resource.get(), deleter(pool, resource));
            }

            /// @copydoc resource_pool::free_unused()
            void free_unused()
            {
                m_free_list.clear();
            }

            /// @copydoc resource_pool::unused_resources()
            uint32_t unused_resources() const
            {
                return static_cast<uint32_t>(m_free_list.size());
            }

            /// This function called when a resource should be added
            /// back into the pool
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

        /// The custom deleter object used by the std::shared_ptr<T>
        /// to de-allocate the object if the pool goes out of
        /// scope. When a std::shared_ptr wants to de-allocate the
        /// object contained it will call the operator() define here.
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
