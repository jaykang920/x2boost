// Copyright (c) 2014 Jae-jun Kang
// See the file COPYING for license details.

#include "x2boost/event.hpp"

#include <boost/thread/once.hpp>

using namespace x2;

namespace
{
    event::tag event_tag;
    boost::once_flag event_once = BOOST_ONCE_INIT;

    void event_init()
    {
        event_tag.set(NULL, 0, 0);
    }
}

const event::tag* event::_tag()
{
    boost::call_once(&event_init, event_once);
    return &event_tag;
}

const cell::tag* event::_type_tag() const
{
    return _tag();
}

// EOF event.cpp
