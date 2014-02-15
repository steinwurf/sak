// Copyright (c) 2012 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include "test_object_xyz_lib_a.hpp"

//
// Fruit
//



//
// Apple
//

std::string apple::color()
{
    return "red";
}

//
// Apple Factory
//

boost::shared_ptr<apple> apple_factory::build(sak::object_registry&)
{
    return boost::make_shared<apple>();
}


//
// Pear
//

std::string pear::color()
{
    return "green";
}

//
// Pear Factory
//

boost::shared_ptr<pear> pear_factory::build(sak::object_registry&)
{
    return boost::make_shared<pear>();
}
