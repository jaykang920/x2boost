// Copyright (c) 2014 Jae-jun Kang
// See the file COPYING for license details.

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
        virtual ~case_interface() = 0;

        virtual void setup(flow_ptr holder) = 0;
        virtual void teardown(flow_ptr holder) = 0;
    };

    class X2BOOST_API caze : public event_sink, public case_interface
    {
    public:
        virtual ~caze() {}

        virtual void setup(flow_ptr holder)
        {
            set_flow(holder);
            setup();
        }

        virtual void teardown(flow_ptr holder)
        {
            teardown();
            cleanup();
        }

    protected:
        virtual void setup() = 0;
        virtual void teardown() = 0;
    };

    class X2BOOST_API case_stack : public case_interface
    {
    public:
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
