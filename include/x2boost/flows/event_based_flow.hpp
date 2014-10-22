// Copyright (c) 2014 Jae-jun Kang
// See the file COPYING for license details.

#ifndef X2BOOST_EVENT_BASED_FLOW_HPP_
#define X2BOOST_EVENT_BASED_FLOW_HPP_

#ifndef X2BOOST_PRE_HPP_
#include "x2boost/pre.hpp"
#endif

#include "x2boost/flow.hpp"
#include "x2boost/queues/synchronized_queue.hpp"

namespace x2
{
    template<class Q = synchronized_event_queue>
    class X2BOOST_API event_based_flow : public flow
    {
    public:
        event_based_flow() {}

        virtual ~event_based_flow() {}

        virtual void feed(event_ptr e) { queue_.enqueue(e); }

        void run()
        {
            current_flow_.reset(this);
            handler_chain_.reset(new handler_chain_type);

            while (true)
            {
                event_ptr e = queue_.dequeue();
                if (!e)
                {
                    break;
                }
                //dispatch(e);
            }

            handler_chain_.reset();
            current_flow_.release();
        }

    protected:
        Q queue_;
    };
}

#endif  // X2BOOST_EVENT_BASED_FLOW_HPP_
