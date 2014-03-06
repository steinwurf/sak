// Copyright (c) 2014 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

// This implementation was inspired by the following discussions:
// http://stackoverflow.com/questions/15024223
// http://stackoverflow.com/questions/21271728
// https://groups.google.com/a/isocpp.org/forum/#!topic/std-proposals/CxpGVY1APcs

#pragma once

#include <cstdint>

#include <functional>

// Define a custom placeholder
namespace sak
{
    namespace detail
    {
        template<int I> struct placeholder {};
    }
}

// Make std::bind aware of the custom placeholder
namespace std
{
    template<int I>
    struct is_placeholder< ::sak::detail::placeholder<I> > :
        std::integral_constant<int, I>
    {};
}

namespace sak
{
    // Do not expose implementation details to users of this header file
    namespace detail
    {
        // Use the indices trick to build a pack of indices
        // http://loungecpp.wikidot.com/tips-and-tricks%3aindices

        template <size_t... Is>
        struct indices {};

        template <size_t N, std::size_t... Is>
        struct build_indices : build_indices<N-1, N-1, Is...> {};

        template <size_t... Is>
        struct build_indices<0, Is...> : indices<Is...> {};

        template<std::size_t... Is, class F, class... Args>
        auto easy_bind(indices<Is...>, F const& f, Args&&... args) ->
            decltype(std::bind(
                f, std::forward<Args>(args)..., placeholder<Is + 1>{}...))
        {
            return std::bind(
                f, std::forward<Args>(args)..., placeholder<Is + 1>{}...);
        }
    }

    // Bind to an std::function instance
    template<class R, class... FArgs, class... Args>
    auto easy_bind(std::function<R(FArgs...)> f, Args&&... args) ->
        decltype(detail::easy_bind(
            detail::build_indices<sizeof...(FArgs) - sizeof...(Args)>{},
            f, std::forward<Args>(args)...))
    {
        return detail::easy_bind(
            detail::build_indices<sizeof...(FArgs) - sizeof...(Args)>{},
            f, std::forward<Args>(args)...);
    }

    // Bind to a free function
    template<class R, class... FArgs, class... Args>
    auto easy_bind(R (*f)(FArgs...), Args&&... args) ->
        decltype(detail::easy_bind(
            detail::build_indices<sizeof...(FArgs) - sizeof...(Args)>{},
            f, std::forward<Args>(args)...))
    {
        return detail::easy_bind(
            detail::build_indices<sizeof...(FArgs) - sizeof...(Args)>{},
            f, std::forward<Args>(args)...);
    }

    // Bind to a member function
    template <typename R, typename T, typename... FArgs, typename... Args>
    auto easy_bind(R (T::*mf)(FArgs...), Args&&... args) ->
        decltype(detail::easy_bind(
            detail::build_indices<(sizeof...(FArgs) + 1) - sizeof...(Args)>(),
            mf, std::forward<Args>(args)...))
    {
        // The extra argument is the object pointer (this)
        static_assert(sizeof...(Args) <= sizeof...(FArgs) + 1,
                      "Too many arguments to easy_bind");

        return detail::easy_bind(
            detail::build_indices<(sizeof...(FArgs) + 1) - sizeof...(Args)>(),
            mf, std::forward<Args>(args)...);
    }
}
