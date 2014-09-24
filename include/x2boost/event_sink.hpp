// Copyright (c) 2014 Jae-jun Kang
// See the file COPYING for license details.

#ifndef X2BOOST_EVENT_SINK_HPP_
#define X2BOOST_EVENT_SINK_HPP_

#ifndef X2BOOST_PRE_HPP_
#include "x2boost/pre.hpp"
#endif

#include <boost/weak_ptr.hpp>

namespace x2
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
        boost::weak_ptr<x2::flow> f;
    };
}

#endif  // X2BOOST_EVENT_SINK_HPP_
