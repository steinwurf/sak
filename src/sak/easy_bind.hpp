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
        inline auto easy_bind(indices<Is...>, F const& f, Args&&... args) ->
            decltype(std::bind(
                f, std::forward<Args>(args)..., placeholder<Is + 1>{}...))
        {
            return std::bind(
                f, std::forward<Args>(args)..., placeholder<Is + 1>{}...);
        }
    }

    /// Bind to a std::function instance.
    ///
    /// Example:
    ///
    ///     void free_function(uint32_t a, uint32_t b, uint32_t c)
    ///     {
    ///         // Some fancy calculations
    ///     }
    ///
    ///     std::function<void(uint32_t,uint32_t,uint32_t)> f1 =
    ///         std::bind(&free_function, _1, _2, _3);
    ///
    ///     // Bind f2 to the std::function f1
    ///     auto f2 = sak::easy_bind(f1);
    ///
    template<class R, class... FArgs, class... Args>
    inline auto easy_bind(std::function<R(FArgs...)> f, Args&&... args) ->
        decltype(detail::easy_bind(
            detail::build_indices<sizeof...(FArgs) - sizeof...(Args)>{},
            f, std::forward<Args>(args)...))
    {
        return detail::easy_bind(
            detail::build_indices<sizeof...(FArgs) - sizeof...(Args)>{},
            f, std::forward<Args>(args)...);
    }

    /// Bind to a free function.
    ///
    /// Example:
    ///
    ///     void free_function(uint32_t a, uint32_t b, uint32_t c)
    ///     {
    ///         // Some fancy calculations
    ///     }
    ///
    ///     // Bind f to free_function
    ///     auto f = sak::easy_bind(&free_function);
    ///
    ///     // Bind g to free_function specifying the first argument
    ///     auto g = sak::easy_bind(&free_function, 1337);
    ///
    template<class R, class... FArgs, class... Args>
    inline auto easy_bind(R (*f)(FArgs...), Args&&... args) ->
        decltype(detail::easy_bind(
            detail::build_indices<sizeof...(FArgs) - sizeof...(Args)>{},
            f, std::forward<Args>(args)...))
    {
        return detail::easy_bind(
            detail::build_indices<sizeof...(FArgs) - sizeof...(Args)>{},
            f, std::forward<Args>(args)...);
    }

    /// Bind to a member function.
    ///
    /// Example:
    ///
    ///     struct foo
    ///     {
    ///         void member_function(uint32_t a, uint32_t b, uint32_t c)
    ///         {
    ///             // Some fancy calculations
    ///         }
    ///     };
    ///
    ///     foo the_foo;
    ///
    ///     // Bind f to the_foo's member_function
    ///     auto f = sak::easy_bind(&foo::member_function, &the_foo);
    ///
    ///     // Bind g to foo's member_function specifying the first argument
    ///     auto g = sak::easy_bind(&foo::member_function, &the_foo,
    ///                             1337);
    ///
    template <typename R, typename T, typename... FArgs, typename... Args>
    inline auto easy_bind(R (T::*mf)(FArgs...), Args&&... args) ->
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

    /// Bind to const member function
    template <typename R, typename T, typename... FArgs, typename... Args>
    inline auto easy_bind(R (T::*mf)(FArgs...) const, Args&&... args) ->
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







    //////////////////////////////////////////////////////

    namespace detail
    {
        template<typename T>
        struct get_signature_impl;

        template<typename C, typename R, typename... A>
        struct get_signature_impl<R(C::*)(A...)>
        {
            typedef R type(A...);
        };

        template<typename C, typename R, typename... A>
        struct get_signature_impl<R(C::*)(A...) const>
        {
            typedef R type(A...);
        };

        template<typename C, typename R, typename... A>
        struct get_signature_impl<R(C::*)(A...) volatile>
        {
            typedef R type(A...);
        };

        template<typename C, typename R, typename... A>
        struct get_signature_impl<R(C::*)(A...) const volatile>
        {
            typedef R type(A...);
        };

        template<typename R, typename... A>
        struct get_signature_impl<R(A...)>
        {
            typedef R type(A...);
        };

        template<typename R, typename... A>
        struct get_signature_impl<R(&)(A...)>
        {
            typedef R type(A...);
        };

        template<typename R, typename... A>
        struct get_signature_impl<R(*)(A...)>
        {
            typedef R type(A...);
        };

        template<typename T> using get_signature =
            typename get_signature_impl<T>::type;

        template<typename F> using make_function_type =
            std::function<get_signature<F>>;

        template<typename F> make_function_type<F> make_function(F f) {
            return make_function_type<F>(f); }
    }

    namespace detail
    {
        template<class F, class... Args>
        std::function<void()> try_bind(F, Args..., char)
        {
            return std::function<void()>();
        }

        template<class F, class... Args>
        auto try_bind(F f, Args... args, int) ->
            decltype(sak::easy_bind(f, args...), make_function(f))
        {

            make_function_type<F> v = sak::easy_bind(f, args...);

            return v;//sak::easy_bind(f, args...);
        }
    }

    template<class F, class... Args>
    auto try_bind(F f, Args... args) ->
        decltype(detail::try_bind<F, Args...>(f, args..., 0))
    {
        return detail::try_bind<F,Args...>(f, args..., 0);
    }

}
