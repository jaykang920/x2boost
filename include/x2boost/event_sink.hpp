// Copyright (c) 2014-2017 Jae-jun Kang
// See the file LICENSE for details.

#ifndef X2BOOST_EVENT_SINK_HPP_
#define X2BOOST_EVENT_SINK_HPP_

#ifndef X2BOOST_PRE_HPP_
#include "x2boost/pre.hpp"
#endif

#include <boost/weak_ptr.hpp>

namespace x2boost
{
    class X2BOOST_API event_sink
    {
    public:
        virtual ~event_sink()
        {
            cleanup();
        }

        flow_ptr flow() const { return f.lock(); }
        void set_flow(flow_ptr value) { f = value; }

    protected:
        void cleanup()
        {
            if (flow_ptr p = f.lock())
            {
                // ...
            }
        }

    private:
        boost::weak_ptr<x2boost::flow> f;
    };
}

#endif  // X2BOOST_EVENT_SINK_HPP_
