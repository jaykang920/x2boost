// Copyright (c) 2014-2015 Jae-jun Kang
// See the file LICENSE for details.

#include "x2boost/event_factory.hpp"


using namespace x2;

event_factory& event_factory::instance()
{
    static event_factory instance;
    return instance;
}

// EOF event_factory.cpp
