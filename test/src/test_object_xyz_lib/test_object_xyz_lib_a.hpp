// Copyright (c) 2012 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <sak/object_registry.hpp>

class fruit
{
public:
    virtual std::string color() = 0;
};

class apple : public fruit
{
public:
    std::string color();
};
SAK_DEFINE_PARENT(apple, fruit)



class apple_factory
{
public:
    boost::shared_ptr<apple> build(sak::object_registry&);
};

class pear : public fruit
{
public:
    std::string color();
};
SAK_DEFINE_PARENT(pear, fruit)

class pear_factory
{
public:
    boost::shared_ptr<pear> build(sak::object_registry&);
};
