// Copyright (c) 2014 Jae-jun Kang
// See the file COPYING for license details.

#ifndef X2BOOST_FLOW_HPP_
#define X2BOOST_FLOW_HPP_

#ifndef X2BOOST_PRE_HPP_
#include "x2boost/pre.hpp"
#endif

#include <boost/enable_shared_from_this.hpp>

#include "x2boost/case.hpp"

namespace x2
{
    class X2BOOST_API flow
      : public boost::enable_shared_from_this<flow>, private boost::noncopyable
    {
    public:
        virtual ~flow() {}

        virtual void feed(event_ptr e) = 0;

        flow_ptr add(case_ptr c)
        {
            case_stack_.add(c);
            return shared_from_this();
        }

        flow_ptr remove(case_ptr c)
        {
            case_stack_.remove(c);
            return shared_from_this();
        }

        void startup() {}
        void shutdown()
        {
            std::cout << "flow::shutdown" << std::endl;
        }

        /// Makes this flow subscribe to the specified channel.
        flow& subscribe_to(const char* channel) const;
        /// Makes this flow unsubscribe from the specified channel.
        flow& unsubscribe_from(const char* channel) const;

    protected:
        virtual void setup()
        {

        }
        virtual void teardown() {}

        case_stack case_stack_;

        mutable boost::mutex mutex_;
    };
}

#endif  // X2BOOST_FLOW_HPP_
