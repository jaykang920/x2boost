// Copyright (c) 2014-2016 Jae-jun Kang
// See the file LICENSE for details.

#ifndef X2BOOST_ASIO_TCP_SESSION_HPP_
#define X2BOOST_ASIO_TCP_SESSION_HPP_

#ifndef X2BOOST_PRE_HPP_
#include "x2boost/pre.hpp"
#endif

#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>

#include "x2boost/buffer.hpp"
#include "x2boost/deserializer.hpp"
#include "x2boost/event.hpp"
#include "x2boost/event_factory.hpp"
#include "x2boost/hub.hpp"
#include "x2boost/serializer.hpp"
#include "x2boost/links/asio_tcp_link.hpp"

namespace x2
{
    class X2BOOST_API asio_tcp_link_session
        : public link_session, public boost::enable_shared_from_this<asio_tcp_link_session>
    {
    public:
        typedef boost::shared_ptr<asio_tcp_link_session> pointer;

        asio_tcp_link_session(asio_tcp_link* link)
            : link_(link), socket_(asio_link::io_service()) { }

        //static pointer _new() { return pointer(new asio_tcp_link_session); }

        virtual void close() {}
        virtual void send(event_ptr e)
        {
            {
                serializer ser(send_buffer_);
                e->_serialize(ser);
            }

            boost::uint32_t header = 0;
            header |= (send_buffer_.length() << 1);

            boost::int32_t header_length = serializer::write_variable(header_bytes_, header);

            send_buffer_list_.clear();
            send_buffer_list_.push_back(boost::asio::buffer(header_bytes_, header_length));
            send_buffer_.list_occupied_buffers(send_buffer_list_);

            start_send();
        }

        boost::asio::ip::tcp::socket& socket() { return socket_; }

        void start_send()
        {
            socket_.async_send(send_buffer_list_,
                boost::bind(&asio_tcp_link_session::handle_send, shared_from_this(),
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));
        }

        void handle_send(const boost::system::error_code& error,
            std::size_t bytes_transferred)
        {
            log::info() << "sent " << bytes_transferred << " byte(s)" << std::endl;

            send_buffer_.trim();
        }

        void start_receive()
        {
            recv_buffer_list_.clear();
            recv_buffer_.list_mutable_buffers(recv_buffer_list_);

            socket_.async_receive(
                recv_buffer_list_,
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
                on_disconnect();
                return;
            }
            log::info() << "received " << bytes_transferred << " byte(s)" << std::endl;

            // TODO FIXME temp
            recv_buffer_.stretch((int)bytes_transferred);
            // beginning?
            recv_buffer_.pos(0);
            boost::uint32_t header;
            int header_length;
            header_length = deserializer::read_variable_internal(recv_buffer_, header);
            recv_buffer_.shrink(header_length);
            int length_to_receive = (int)(header >> 1);

            if (recv_buffer_.length() < length_to_receive)
            {
                // continue to receive more
                //start_receive();
                return;
            }

            // loop to process multiple events received at once

            recv_buffer_.mark_to_read(length_to_receive);

            {
                deserializer des(recv_buffer_);
                event_ptr retrieved = event_factory::create(des);
                if (retrieved)
                {
                    retrieved->_deserialize(des);

                    log::info() << retrieved->_string() << std::endl;

                    retrieved->_handle(handle_);

                    // preproc
                    
                    hub::post(retrieved);
                }
                else
                {
                    log::error("event retrieval error");
                }
            }

            recv_buffer_.trim();

            start_receive();
        }

    protected:
        virtual void on_disconnect()
        {
            log::info() << link_->name() << " " << handle() << " disconnected from " << socket_.remote_endpoint() << std::endl;
            link_->on_disconnect(shared_from_this());
        }

        asio_tcp_link* link_;

        boost::asio::ip::tcp::socket socket_;

        buffer recv_buffer_;
        buffer send_buffer_;

        std::vector<boost::asio::const_buffer> send_buffer_list_;
        std::vector<boost::asio::mutable_buffer> recv_buffer_list_;

        byte_t header_bytes_[5];
    };
}

#endif  // X2BOOST_ASIO_TCP_SESSION_HPP_
