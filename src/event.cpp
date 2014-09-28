// Copyright (c) 2014 Jae-jun Kang
// See the file COPYING for license details.

#include "x2boost/event.hpp"

#include <boost/thread/once.hpp>

using namespace x2;

namespace
{
    event::tag t;
    boost::once_flag once = BOOST_ONCE_INIT;

    void init()
    {
        t.set(NULL, 0, 0);
    }
}

const event::tag* event::_tag()
{
    boost::call_once(&init, once);
    return &t;
}

const cell::tag* event::type_tag() const
{
    return _tag();
}

// EOF event.cpp
