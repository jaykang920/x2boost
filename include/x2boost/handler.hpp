// Copyright (c) 2014 Jae-jun Kang
// See the file COPYING for license details.

#ifndef X2BOOST_HANDLER_HPP_
#define X2BOOST_HANDLER_HPP_

#ifndef X2BOOST_PRE_HPP_
#include "x2boost/pre.hpp"
#endif

namespace x2
{
    struct X2BOOST_API handler
    {
        virtual void invoke(event_ptr e) = 0;
    };
}

#endif  // X2BOOST_HANDLER_HPP_
