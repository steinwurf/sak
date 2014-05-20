// Copyright (c) 2014 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include "easy_bind.hpp"

namespace sak
{
    // Do not expose implementation details to users of this header file

    // namespace detail
    // {
    //     template<typename T>
    //     struct get_signature_impl;

    //     template<typename C, typename R, typename... A>
    //     struct get_signature_impl<R(C::*)(A...)>
    //     {
    //         typedef R type(A...);
    //     };

    //     template<typename C, typename R, typename... A>
    //     struct get_signature_impl<R(C::*)(A...) const>
    //     {
    //         typedef R type(A...);
    //     };

    //     template<typename C, typename R, typename... A>
    //     struct get_signature_impl<R(C::*)(A...) volatile>
    //     {
    //         typedef R type(A...);
    //     };

    //     template<typename C, typename R, typename... A>
    //     struct get_signature_impl<R(C::*)(A...) const volatile>
    //     {
    //         typedef R type(A...);
    //     };

    //     template<typename R, typename... A>
    //     struct get_signature_impl<R(A...)>
    //     {
    //         typedef R type(A...);
    //     };

    //     template<typename R, typename... A>
    //     struct get_signature_impl<R(&)(A...)>
    //     {
    //         typedef R type(A...);
    //     };

    //     template<typename R, typename... A>
    //     struct get_signature_impl<R(*)(A...)>
    //     {
    //         typedef R type(A...);
    //     };

    //     template<typename T> using get_signature =
    //         typename get_signature_impl<T>::type;

    //     template<typename F> using make_function_type =
    //         std::function<get_signature<F>>;

    //     template<typename F> make_function_type<F> make_function(F f) {
    //         return make_function_type<F>(f); }
    // }

    namespace detail
    {
        /// Helper struct which serves a return value if we were not able
        /// to bind to the specified function
        struct not_valid_bind
        { };

        template<class B, class F>
        not_valid_bind bind_method(F&, char)
        {
            return not_valid_bind();
        }

        template<class B, class F>
        auto bind_method(F& f, int) -> decltype(B::bind(f))
        {
            return B::bind(f);
        }
    }

    /// The optional_bind utility was designed for cases where we want
    /// to bind to a specific member function if that function is
    /// available otherwise we want to skip the bind.
    ///
    /// optional_bind is designed with a quite narrow scope, but it
    /// can probably be extended to server more purposes if needed.
    ///
    /// The following example shows optional_bind in action:
    ///
    ///     class foo
    ///     {
    ///
    ///
    ///
    template<class B, class F>
    auto optional_bind(F& f) ->
        decltype(detail::bind_method<B, F>(f, 0))
    {
        return detail::bind_method<B, F>(f, 0);
    }

    /// Checks whether the bind was succesfull
    template<class T>
    bool is_bind_valid(const T&)
    {
        return true;
    }

    template<>
    bool is_bind_valid<detail::not_valid_bind>(const detail::not_valid_bind&)
    {
        return false;
    }


}
