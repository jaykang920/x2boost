// Copyright (c) 2014-2015 Jae-jun Kang
// See the file LICENSE for details.

#ifndef X2BOOST_ASIO_TCP_SERVER_HPP_
#define X2BOOST_ASIO_TCP_SERVER_HPP_

#ifndef X2BOOST_PRE_HPP_
#include "x2boost/pre.hpp"
#endif

#include <boost/bind.hpp>

#include "x2boost/links/asio_link.hpp"
#include "x2boost/links/asio_tcp_link_session.hpp"

namespace x2
{
    class X2BOOST_API asio_tcp_server : public asio_link
    {
    public:
        explicit asio_tcp_server(const std::string& name)
            : asio_link(name), acceptor_(io_service_) {}

        virtual void close()
        {
            acceptor_.close();
        }

        void listen(int port)
        {
            boost::asio::ip::tcp::endpoint ep(boost::asio::ip::tcp::v4(), (short)port);
            acceptor_.open(ep.protocol());
            acceptor_.bind(ep);
            acceptor_.listen();

            log::info() << name() << " listening on " << port << std::endl;

            start_accept();
        }

        void start_accept()
        {
            asio_tcp_link_session::pointer session = asio_tcp_link_session::_new();

            acceptor_.async_accept(session->socket(),
                boost::bind(&asio_tcp_server::handle_accept, this, session,
                    boost::asio::placeholders::error)
                );

        }

        void handle_accept(asio_tcp_link_session::pointer session,
            const boost::system::error_code& error)
        {
            if (error)
            {
                log::error() << name() << " accept error " << error.message() << std::endl;
                return;
            }

            log::debug() << name() << " accepted " << session->socket().remote_endpoint() << std::endl;

            session_ = session;

            session->start_receive();
            start_accept();
        }

        void send(event_ptr e)
        {
            session_->send(e);
        }

    protected:
        boost::asio::ip::tcp::acceptor acceptor_;

        asio_tcp_link_session::pointer session_;
    };
}

#endif  // X2BOOST_ASIO_TCP_SERVER_HPP_
