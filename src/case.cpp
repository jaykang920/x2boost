// Copyright (c) 2014 Jae-jun Kang
// See the file COPYING for license details.

#include "x2boost/case.hpp"

#include <boost/bind.hpp>
#include <boost/thread/locks.hpp>

using namespace x2;

void case_stack::add(case_ptr c)
{
    boost::mutex::scoped_lock lock(mutex_);
    if (std::find(cases_.begin(), cases_.end(), c) == cases_.end())
    {
        cases_.push_back(c);
    }
}

void case_stack::remove(case_ptr c)
{
    boost::mutex::scoped_lock lock(mutex_);
    cases_.erase(std::remove(cases_.begin(), cases_.end(), c), cases_.end());
}

void case_stack::setup(flow_ptr holder)
{
    std::vector<case_ptr> snapshot;
    {
        boost::mutex::scoped_lock lock(mutex_);
        snapshot = cases_;
    }
    std::for_each(snapshot.begin(), snapshot.end(),
        boost::bind(&case_interface::setup, _1, holder));
}

void case_stack::teardown(flow_ptr holder)
{
    std::vector<case_ptr> snapshot;
    {
        boost::mutex::scoped_lock lock(mutex_);
        snapshot = cases_;
    }
    std::for_each(snapshot.rbegin(), snapshot.rend(),
        boost::bind(&case_interface::teardown, _1, holder));
}

// EOF case.cpp
