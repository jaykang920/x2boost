// Copyright (c) 2014 Jae-jun Kang
// See the file COPYING for license details.

#include "x2boost/flow.hpp"

#include "x2boost/hub.hpp"

using namespace x2;

void flow::subscribe_to(const char* channel) const
{
    hub::instance().subscribe(boost::const_pointer_cast<flow>(shared_from_this()), channel);
}

void flow::unsubscribe_from(const char* channel) const
{
    hub::instance().unsubscribe(boost::const_pointer_cast<flow>(shared_from_this()), channel);
}

// EOF flow.cpp
