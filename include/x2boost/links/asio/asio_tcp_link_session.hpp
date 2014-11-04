// Copyright (c) 2014 Jae-jun Kang
// See the file COPYING for license details.

#ifndef X2BOOST_ASIO_TCP_SESSION_HPP_
#define X2BOOST_ASIO_TCP_SESSION_HPP_

#ifndef X2BOOST_PRE_HPP_
#include "x2boost/pre.hpp"
#endif

#include <boost/asio.hpp>

#include "x2boost/links/asio/asio_link.hpp"
#include "x2boost/links/asio/asio_link_session.hpp"

namespace x2
{
    class X2BOOST_API asio_tcp_link_session : public asio_link_session
    {
    public:
        typedef boost::shared_ptr<asio_tcp_link_session> pointer;

        static pointer _new() { return pointer(new asio_tcp_link_session); }

        virtual void close() {}
        virtual void send(event_ptr e) {}

        boost::asio::ip::tcp::socket& socket() { return socket_; }

        void start_send()
        {
            socket_.async_send(boost::asio::buffer(txbuf_.data()),
                boost::bind(&asio_tcp_link_session::handle_send, this,
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));
        }

        void handle_send(const boost::system::error_code& error,
            std::size_t bytes_transferred)
        {
            log::info() << "sent " << bytes_transferred << " byte(s)" << std::endl;
        }

        void start_receive()
        {
            boost::asio::streambuf::mutable_buffers_type mutable_buffer =
                rxbuf_.prepare(4096);
            socket_.async_receive(boost::asio::buffer(mutable_buffer),
                boost::bind(&asio_tcp_link_session::handle_receive, this,
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
        }

        void handle_receive(const boost::system::error_code& error,
            std::size_t bytes_transferred)
        {
            log::info() << "received " << bytes_transferred << " byte(s)" << std::endl;
            start_receive();
        }

    protected:
        asio_tcp_link_session() : socket_(asio_link::io_service()) {}

        boost::asio::ip::tcp::socket socket_;

        boost::asio::streambuf rxbuf_;
        boost::asio::streambuf txbuf_;
    };
}

#endif  // X2BOOST_ASIO_TCP_SESSION_HPP_
