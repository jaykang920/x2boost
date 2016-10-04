// Copyright (c) 2014-2016 Jae-jun Kang
// See the file LICENSE for details.

#ifndef X2BOOST_CASE_HPP_
#define X2BOOST_CASE_HPP_

#ifndef X2BOOST_PRE_HPP_
#include "x2boost/pre.hpp"
#endif

#include <boost/thread/mutex.hpp>

#include "x2boost/event_sink.hpp"

namespace x2
{
    struct X2BOOST_API case_interface
    {
        virtual ~case_interface() {}

        virtual void setup(flow_ptr holder) = 0;
        virtual void teardown(flow_ptr holder) = 0;
    
    protected:
        case_interface() {}
    };

    class X2BOOST_API caze : public event_sink, public case_interface
    {
    public:
        virtual ~caze() { }

        virtual void setup(flow_ptr holder);
        virtual void teardown(flow_ptr holder);

    protected:
        caze() { }

        // Intializes this case on startup.
        virtual void setup() { }
        // Cleans up this case on shutdown.
        virtual void teardown() { }
    };

    class X2BOOST_API case_stack : public case_interface
    {
    public:
        case_stack() {}
        virtual ~case_stack() {}

        void add(case_ptr c);
        void remove(case_ptr c);

        virtual void setup(flow_ptr holder);
        virtual void teardown(flow_ptr holder);

    private:
        std::vector<case_ptr> cases_;

        mutable boost::mutex mutex_;
    };
}

#endif  // X2BOOST_CASE_HPP_
