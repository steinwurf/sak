// Copyright (c) 2012 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

namespace sak
{
    /// @brief Checks whether a type is regular.
    /// @todo docs + tests
    template<class T>
    struct is_regular :
        std::integral_constant<bool,
        std::is_default_constructible<T>::value &&
        std::is_copy_constructible<T>::value &&
        std::is_move_constructible<T>::value &&
        std::is_copy_assignable<T>::value &&
        std::is_move_assignable<T>::value>
    { };
}
