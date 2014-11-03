// Copyright (c) 2012 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <string>
#include <memory>

#include <sak/object_registry.hpp>
#include "test_object_xyz_lib_a.hpp"

class bird
{
public:
    virtual std::string eat() = 0;
};

class duck : public bird
{
public:
    duck(sak::object_registry& registry);
    std::string eat();

private:
    sak::object_registry& m_registry;
};
SAK_DEFINE_PARENT(duck, bird)


class duck_factory
{
public:
    std::shared_ptr<duck> build(sak::object_registry&);
};
