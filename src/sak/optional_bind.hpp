// Copyright (c) 2014 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <functional>

namespace sak
{
// Do not expose implementation details to users of this header file
namespace detail
{
/// Overload of the internal optional_bind. This overload is
/// chosen if the B::bind(f) expression cannot be
/// instantiated. Thanks to SFINAE this means that this
/// overload will be the only valid expression and therefore
/// it will be chosen. If however the bind expression is valid
/// then both overloads will be available, however since the
/// int version does not require an implict conversion of the
/// numeric constant from int to char it will be preferred.
template<class B, class F>
auto optional_bind(F&&, char) -> typename B::result_type
{
    return typename B::result_type();
}

template<class B, class F>
auto optional_bind(F&& f, int) ->
    decltype(B::bind(std::forward<F>(f)), typename B::result_type())
{
    return B::bind(std::forward<F>(f));
}
}

/// The optional_bind utility was designed for cases where we want
/// to bind to a specific member function if that function is
/// available otherwise we want to skip the bind.
///
/// optional_bind is designed with a quite narrow scope, but it
/// can probably be extended to serve more purposes if needed.
///
/// The following example shows optional_bind in action:
///
///     struct foo
///     {
///         void make_coffee()
///         {
///             std::cout << "coffee on its way" << std::endl;
///         }
///     };
///
/// Now somewhere in your code you want to bind to two different
/// member functions, but if they are not there it is ok.
///
/// To do this we need to define two helpers which implement the
/// actual bind (we will utilize the easy_bind helper here to make
/// the code simple but you can also use std::bind):
///
///     struct bind_make_coffee
///     {
///         template<class T>
///         static auto bind(T* t) ->
///             decltype(sak::easy_bind(&T::make_coffee, t))
///         {
///             return sak::easy_bind(&T::make_coffee, t);
///         }
///
///         using result_type = std::function<void()>;
///     };
///
///     struct bind_grind_beans
///     {
///         template<class T>
///         static auto bind(T* t) ->
///             decltype(sak::easy_bind(&T::grind_beans, t))
///         {
///             return sak::easy_bind(&T::grind_beans, t);
///         }
///
///         using result_type = std::function<void()>;
///     };
///
/// With these helpers we can now write the following:
///
///    foo f;
///    auto make_coffee = sak::optional_bind<bind_make_coffee>(&f);
///    auto grind_beans = sak::optional_bind<bind_grind_beans>(&f);
///
///    // Using std::function's built-in operator bool() we can check
///    // if the bind was successful
///    assert(make_coffee);
///    assert(!grind_beans);
///
/// The nice thing about this is that is can be done without
/// having to directly utilize SFINAE tricks directly in the code.
///
/// One thing we have to be careful with is that our bind helpers
/// will get T as we passed it when calling optional_bind so if we
/// did not pass a pointer to foo we would get it by reference
/// e.g.:
///
///     struct bind_make_coffee
///     {
///         template<class T>
///         static auto bind(T& t) ->
///             decltype(sak::easy_bind(&T::make_coffee, &t))
///         {
///             return sak::easy_bind(&T::make_coffee, &t);
///         }
///     };
///
///    Used like this:
///
///    foo f;
///    auto make_coffee = sak::optinal_bind<bind_make_coffee>(f);
///
///    assert(make_coffee);
///
/// The return value of optional_bind is specified by the type
/// result_type defined in the bind helper. The result_type must
/// be default constructable i.e. define a trivial constructor.
///
/// The optional_bind helper takes the following arguments:
///
///  - B which is a type that should provide a static function
///    called bind which should take F as a parameter. Usually we
///    will make bind a template function to allow late binding as
///    per our example above.
///  - F is the object to which we wish to bind
///
template<class B, class F>
auto optional_bind(F&& f) ->
    decltype(detail::optional_bind<B>(std::forward<F>(f), 0))
{
    return detail::optional_bind<B>(std::forward<F>(f), 0);
}
}
