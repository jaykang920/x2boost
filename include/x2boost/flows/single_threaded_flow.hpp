// Copyright (c) 2014-2015 Jae-jun Kang
// See the file LICENSE for details.

#ifndef X2BOOST_SINGLE_THREADED_FLOW_HPP_
#define X2BOOST_SINGLE_THREADED_FLOW_HPP_

#ifndef X2BOOST_PRE_HPP_
#include "x2boost/pre.hpp"
#endif

#include <boost/bind.hpp>
#include <boost/thread.hpp>

#include "x2boost/flows/event_based_flow.hpp"

namespace x2
{
    template<class Q = synchronized_event_queue>
    class X2BOOST_API single_threaded_flow : public event_based_flow<Q>
    {
    public:
        single_threaded_flow() : thread_(NULL) {}

        virtual ~single_threaded_flow() {}

        virtual void startup()
        {
            boost::mutex::scoped_lock lock(flow::mutex_);
            if (thread_) { return; }

            this->setup();
            flow::case_stack_.setup(boost::enable_shared_from_this<flow>::shared_from_this());

            thread_ = new boost::thread(boost::bind(&event_based_flow<Q>::run, this));
        }

        virtual void shutdown()
        {
            boost::mutex::scoped_lock lock(flow::mutex_);
            if (!thread_) { return; }

            // enqueue flow_stop
            event_based_flow<Q>::queue_.close();

            thread_->join();
            delete thread_;
            thread_ = NULL;

            flow::case_stack_.teardown(boost::enable_shared_from_this<flow>::shared_from_this());
            this->teardown();
        }

    protected:
        boost::thread* thread_;
    };
}

#endif  // X2BOOST_SINGLE_THREADED_FLOW_HPP_
