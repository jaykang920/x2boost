// Copyright (c) 2014 Jae-jun Kang
// See the file COPYING for license details.

#ifndef X2BOOST_FLOW_HPP_
#define X2BOOST_FLOW_HPP_

#ifndef X2BOOST_PRE_HPP_
#include "x2boost/pre.hpp"
#endif

namespace x2
{
    class X2BOOST_API caze {};

    class X2BOOST_API flow : private boost::noncopyable
    {
    public:
        virtual ~flow() {}

        void feed(event_ptr e) {
            std::cout << "flow::feed" << std::endl;
        }

        flow* add(caze* c) { return this; }

        void startup() {}
        void shutdown()
        {
            std::cout << "flow::shutdown" << std::endl;
        }

    private:
    };
}

#endif  // X2BOOST_FLOW_HPP_
