// Copyright (c) 2014-2015 Jae-jun Kang
// See the file LICENSE for details.

#ifndef X2BOOST_ASIO_LINK_SESSION_HPP_
#define X2BOOST_ASIO_LINK_SESSION_HPP_

#ifndef X2BOOST_PRE_HPP_
#include "x2boost/pre.hpp"
#endif

#include <boost/asio.hpp>

#include "x2boost/link.hpp"

namespace x2
{
    class X2BOOST_API asio_link_session : public link_session
    {
    public:

    protected:
        asio_link_session() : link_session(0) {}
    };
}

#endif  // X2BOOST_ASIO_LINK_SESSION_HPP_
