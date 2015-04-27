// Copyright (c) 2014-2015 Jae-jun Kang
// See the file LICENSE for details.

#include "x2boost/event_factory.hpp"

#include "x2boost/deserializer.hpp"

using namespace x2;

event_ptr event_factory::create(deserializer& deserializer)
{
    boost::int32_t type_id;
    deserializer.read(type_id);
    return create(type_id);
}

event_factory& event_factory::instance()
{
    static event_factory instance;
    return instance;
}

// EOF event_factory.cpp
