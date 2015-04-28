// Copyright (c) 2014-2015 Jae-jun Kang
// See the file LICENSE for details.

#ifndef X2BOOST_ASIO_TCP_CLIENT_HPP_
#define X2BOOST_ASIO_TCP_CLIENT_HPP_

#ifndef X2BOOST_PRE_HPP_
#include "x2boost/pre.hpp"
#endif

#include <boost/bind.hpp>

#include "x2boost/links/asio_link.hpp"
#include "x2boost/links/asio_tcp_link_session.hpp"

namespace x2
{
    class X2BOOST_API asio_tcp_client : public asio_link
    {
    public:
        explicit asio_tcp_client(const std::string& name) : asio_link(name) {}

        virtual void close()
        {
            //session.close();
        }

        void connect(const boost::asio::ip::tcp::endpoint& ep)
        {
            log::info() << name() << " connecting to " << ep << std::endl;

            start_connect(ep);
        }

        void start_connect(const boost::asio::ip::tcp::endpoint& ep)
        {
            asio_tcp_link_session::pointer session = asio_tcp_link_session::_new();

            log::debug() << "calling async_connect()" << std::endl;

            session->socket().async_connect(ep,
                boost::bind(&asio_tcp_client::handle_connect, this, session,
                boost::asio::placeholders::error)
                );

            log::debug() << "async_connect() returned" << std::endl;
        }

        void handle_connect(asio_tcp_link_session::pointer session,
            const boost::system::error_code& error)
        {
            log::info() << "entered handle_connect()" << std::endl;
            //
            if (error)
            {
                log::error() << name() << " connect error " << error.message() << std::endl;
                return;
            }
            log::debug() << name() << " connected to " << session->socket().remote_endpoint() << std::endl;

            session_ = session;

            session->start_receive();
        }

        void send(event_ptr e)
        {
            session_->send(e);
        }

    protected:
        asio_tcp_link_session::pointer session_;
    };
}

#endif  // X2BOOST_ASIO_TCP_CLIENT_HPP_
