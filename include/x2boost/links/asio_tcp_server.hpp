// Copyright (c) 2014-2017 Jae-jun Kang
// See the file LICENSE for details.

#ifndef X2BOOST_ASIO_TCP_SERVER_HPP_
#define X2BOOST_ASIO_TCP_SERVER_HPP_

#ifndef X2BOOST_PRE_HPP_
#include "x2boost/pre.hpp"
#endif

#include <boost/bind.hpp>

#include "x2boost/links/asio_tcp_link.hpp"
#include "x2boost/links/asio_tcp_link_session.hpp"

namespace x2boost
{
    class X2BOOST_API asio_tcp_server : public asio_tcp_link
    {
    public:
        explicit asio_tcp_server(const std::string& name)
            : asio_tcp_link(name), acceptor_(io_service_) {}

        virtual void close()
        {
            acceptor_.close();
            sessions_.clear();
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
            asio_tcp_link_session::pointer session(new asio_tcp_link_session(this));

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
                log::error() << name() << " " << session->handle() << " accept error " << error.value() << " " << error.message() << std::endl;
                return;
            }

            log::debug() << name() << " " << session->handle() << " accepted " << session->socket().remote_endpoint() << std::endl;

            {
                boost::unique_lock<boost::shared_mutex> wlock(shared_mutex_);
                sessions_.insert(map_type::value_type(session->handle(), session));
            }

            {
                boost::asio::ip::tcp::no_delay no_delay(true);
                session->socket().set_option(no_delay);
            }

            session->start_receive();
            start_accept();
        }

        virtual void on_disconnect(asio_tcp_link_session::pointer session)
        {
            asio_tcp_link::on_disconnect(session);
            boost::unique_lock<boost::shared_mutex> wlock(shared_mutex_);
            sessions_.erase(session->handle());
        }

        void send(event_ptr e)
        {
            if (e->_handle() == 0)
            {
                return;
            }
            boost::shared_lock<boost::shared_mutex> rlock(shared_mutex_);
            map_type::iterator it = sessions_.find(e->_handle());
            if (it == sessions_.end())
            {
                return;
            }
            it->second->send(e);
        }

    protected:
        typedef boost::unordered_map<int, asio_tcp_link_session::pointer> map_type;

        boost::asio::ip::tcp::acceptor acceptor_;

        map_type sessions_;

        mutable boost::shared_mutex shared_mutex_;
    };
}

#endif  // X2BOOST_ASIO_TCP_SERVER_HPP_
