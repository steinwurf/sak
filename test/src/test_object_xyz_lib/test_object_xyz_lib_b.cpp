// Copyright (c) 2012 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include "test_object_xyz_lib_b.hpp"

#include <memory>
#include <string>

//
// Bird
//

//
// Duck
//

duck::duck(sak::object_registry& registry) :
    m_registry(registry)
{
}

std::string duck::eat()
{
    auto f = m_registry.build<fruit>();
    //return std::string("");
    return std::string("duck eats fruit which is ").append(f->color());
}

//
// Duck Factory
//

std::shared_ptr<duck> duck_factory::build(sak::object_registry& registry)
{
    return std::make_shared<duck>(registry);
}
