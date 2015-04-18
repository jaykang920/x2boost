// Copyright (c) 2014-2015 Jae-jun Kang
// See the file LICENSE for details.

#ifndef X2BOOST_ASIO_TCP_SESSION_HPP_
#define X2BOOST_ASIO_TCP_SESSION_HPP_

#ifndef X2BOOST_PRE_HPP_
#include "x2boost/pre.hpp"
#endif

#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>

#include "x2boost/links/asio_link.hpp"
#include "x2boost/links/asio_link_session.hpp"

namespace x2
{
    class X2BOOST_API asio_tcp_link_session
        : public asio_link_session, public boost::enable_shared_from_this<asio_tcp_link_session>
    {
    public:
        typedef boost::shared_ptr<asio_tcp_link_session> pointer;

        static pointer _new() { return pointer(new asio_tcp_link_session); }

        virtual void close() {}
        virtual void send(event_ptr e) {}

        boost::asio::ip::tcp::socket& socket() { return socket_; }

        void start_send()
        {
            boost::asio::streambuf::const_buffers_type const_buffers =
                txbuf_.data();
            socket_.async_send(boost::asio::buffer(const_buffers),
                boost::bind(&asio_tcp_link_session::handle_send, shared_from_this(),
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
           // boost::asio::streambuf::mutable_buffers_type mutable_buffers =
                //rxbuf_.prepare(4096);

            //log::warning() << mutable_buffers.begin() << " " << mutable_buffers.end() << std::endl;
            //log::warning() << rxbuf_.size() << std::endl;

            socket_.async_receive(
                boost::asio::buffer(buf_, 4096),
                //boost::asio::buffer(mutable_buffers),
                //boost::asio::transfer_at_least(1),
                boost::bind(&asio_tcp_link_session::handle_receive, shared_from_this(),
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
        }

        void handle_receive(const boost::system::error_code& error,
            std::size_t bytes_transferred)
        {
            if (bytes_transferred == 0)
            {
                // disconnect
                return;
            }
            log::info() << "received " << bytes_transferred << " byte(s)" << std::endl;
            //rxbuf_.commit(bytes_transferred);
            start_receive();
        }

    protected:
        asio_tcp_link_session() : socket_(asio_link::io_service()) {}

        boost::asio::ip::tcp::socket socket_;

        boost::asio::streambuf rxbuf_;
        boost::asio::streambuf txbuf_;
        char buf_[4096];
    };
}

#endif  // X2BOOST_ASIO_TCP_SESSION_HPP_
