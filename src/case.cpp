// Copyright (c) 2014-2015 Jae-jun Kang
// See the file LICENSE for details.

#include "x2boost/case.hpp"

#include <boost/bind.hpp>
#include <boost/thread/locks.hpp>

#include "x2boost/flow.hpp"

using namespace x2;

void caze::setup(flow_ptr holder)
{
    set_flow(holder);

    boost::thread_specific_ptr<x2::flow>& current_flow = flow::current_flow();
    x2::flow* backup = current_flow.release();
    current_flow.reset(holder.get());

    setup();

    current_flow.release();
    current_flow.reset(backup);
}

void caze::teardown(flow_ptr holder)
{
    boost::thread_specific_ptr<x2::flow>& current_flow = flow::current_flow();
    x2::flow* backup = current_flow.release();
    current_flow.reset(holder.get());

    teardown();

    current_flow.release();
    current_flow.reset(backup);

    cleanup();
}

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
