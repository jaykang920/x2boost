// Copyright (c) 2014-2017 Jae-jun Kang
// See the file LICENSE for details.

#ifndef X2BOOST_THREADLESS_FLOW_HPP_
#define X2BOOST_THREADLESS_FLOW_HPP_

#ifndef X2BOOST_PRE_HPP_
#include "x2boost/pre.hpp"
#endif

#include <boost/bind.hpp>
#include <boost/thread.hpp>

#include "x2boost/flows/event_based_flow.hpp"

namespace x2boost
{
    template<class Q = synchronized_event_queue>
    class X2BOOST_API threadless_flow : public event_based_flow<Q>
    {
    public:
        threadless_flow() : running_(false) {}

        virtual ~threadless_flow() {}

        virtual void startup()
        {
            boost::mutex::scoped_lock lock(flow::mutex_);
            if (running_) { return; }

            this->setup();
            flow::case_stack_.setup(boost::enable_shared_from_this<flow>::shared_from_this());

            running_ = true;
        }

        virtual void shutdown()
        {
            boost::mutex::scoped_lock lock(flow::mutex_);
            if (!running_) { return; }

            // enqueue flow_stop
            event_based_flow<Q>::queue_.close();

            running_ = false;

            flow::case_stack_.teardown(boost::enable_shared_from_this<flow>::shared_from_this());
            this->teardown();
        }

        event_ptr try_dispatch()
        {
            event_ptr e;

            if (queue_.try_dequeue(&e))
            {
                dispatch(e);
            }
            return e;
        }

        size_t try_dispatch_all()
        {
            event_ptr e;

            deququed_.clear();
            while (queue_.try_dequeue(&e))
            {
                deqeueud_.add(e);
            }
            size_t count = dequeued_.size();
            if (count != 0)
            {
                for (int i = 0; i < count; ++i)
                {
                    dispatch(dequeued_[i]);
                }
                dequeued_.clear();
            }
            return count;
        }

    protected:
        bool running_;
        std::vector<event_ptr> dequeued_;
    };
}

#endif  // X2BOOST_THREADLESS_FLOW_HPP_
