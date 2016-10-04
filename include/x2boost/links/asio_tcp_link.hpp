// Copyright (c) 2014-2016 Jae-jun Kang
// See the file LICENSE for details.

#ifndef X2BOOST_ASIO_TCP_LINK_HPP_
#define X2BOOST_ASIO_TCP_LINK_HPP_

#ifndef X2BOOST_PRE_HPP_
#include "x2boost/pre.hpp"
#endif

#include "x2boost/links/asio_link.hpp"

namespace x2
{
    class asio_tcp_link_session;

    class X2BOOST_API asio_tcp_link : public asio_link
    {
    public:
        virtual ~asio_tcp_link() {}

        virtual void on_disconnect(boost::shared_ptr<asio_tcp_link_session> session)
        {
            // event noti
        }

    protected:
        asio_tcp_link(const std::string& name) : asio_link(name) {}
    };
}

#endif  // X2BOOST_ASIO_TCP_LINK_HPP_
