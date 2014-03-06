// Copyright (c) 2014 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>

#include <functional>

namespace sak
{
    template<int I> struct placeholder {};
}

namespace std
{
    template<int I>
    struct is_placeholder< ::sak::placeholder<I> > :
        std::integral_constant<int, I>
    {};
}

namespace sak
{
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

    template<class R, class... FArgs, class... Args>
    auto easy_bind(std::function<R(FArgs...)> f, Args&&... args) ->
        decltype(sak::easy_bind(
            build_indices<sizeof...(FArgs) - sizeof...(Args)>{},
            f, std::forward<Args>(args)...))
    {
        return sak::easy_bind(
            build_indices<sizeof...(FArgs) - sizeof...(Args)>{},
            f, std::forward<Args>(args)...);
    }

    template<class R, class... FArgs, class... Args>
    auto easy_bind(R (*f)(FArgs...), Args&&... args) ->
        decltype(sak::easy_bind(
            build_indices<sizeof...(FArgs) - sizeof...(Args)>{},
            f, std::forward<Args>(args)...))
    {
        return sak::easy_bind(
            build_indices<sizeof...(FArgs) - sizeof...(Args)>{},
            f, std::forward<Args>(args)...);
    }

    template <typename R, typename T, typename... FArgs, typename... Args>
    auto easy_bind(R (T::*mf)(FArgs...), Args&&... args) ->
        decltype(sak::easy_bind(std::function<R(T*,FArgs...)>(mf), args...))
    {
        return sak::easy_bind(std::function<R(T*,FArgs...)>(mf), args...);
    }
}
