// Copyright (c) 2014-2016 Jae-jun Kang
// See the file LICENSE for details.

#ifndef X2BOOST_MULTI_THREADED_FLOW_HPP_
#define X2BOOST_MULTI_THREADED_FLOW_HPP_

#ifndef X2BOOST_PRE_HPP_
#include "x2boost/pre.hpp"
#endif

#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/thread.hpp>

#include "x2boost/flows/event_based_flow.hpp"

namespace x2
{
    template<class Q = synchronized_event_queue>
    class X2BOOST_API multi_threaded_flow : public event_based_flow<Q>
    {
    public:
        multi_threaded_flow(int n) {}

        virtual ~multi_threaded_flow() {}

        virtual void startup()
        {
            boost::mutex::scoped_lock lock(mutex_);
            if (!threads_.empty()) { return; }

            setup();
            case_stack_.setup(shared_from_this());

            for (int i = 0; i < n_; ++i)
            {
                boost::thread* t =
                    new boost::thread(boost::bind(&event_based_flow<Q>::run, this));
                threads_.push_back(t);
            }
        }

        virtual void shutdown()
        {
            boost::mutex::scoped_lock lock(mutex_);
            if (threads_.empty()) { return; }

            // enqueue flow_stop
            queue_.close();

            BOOST_FOREACH(boost::thread* t, threads_)
            {
                t->join();
                delete t;
            }
            threads_.clear();

            case_stack_.teardown(shared_from_this());
            teardown();
        }

    protected:
        int n_;
        std::vector<boost::thread*> threads_;
    };
}

#endif  // X2BOOST_MULTI_THREADED_FLOW_HPP_
