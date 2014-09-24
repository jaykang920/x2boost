// Copyright (c) 2014 Jae-jun Kang
// See the file COPYING for license details.

#include "x2boost/flow.hpp"

#include "x2boost/hub.hpp"

using namespace x2;

flow& flow::subscribe_to(const char* channel) const
{
    flow* p = const_cast<flow*>(this);
    hub::instance().subscribe(flow_ptr(p), channel);
    return *p;
}

flow& flow::unsubscribe_from(const char* channel) const
{
    flow* p = const_cast<flow*>(this);
    hub::instance().unsubscribe(flow_ptr(p), channel);
    return *p;
}

// EOF flow.cpp
