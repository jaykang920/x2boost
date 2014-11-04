// Copyright (c) 2014 Jae-jun Kang
// See the file COPYING for license details.

#ifndef X2BOOST_ASIO_LINK_HPP_
#define X2BOOST_ASIO_LINK_HPP_

#ifndef X2BOOST_PRE_HPP_
#include "x2boost/pre.hpp"
#endif

#include <boost/asio.hpp>

#include "x2boost/link.hpp"

namespace x2
{
    class X2BOOST_API asio_link : public link
    {
    public:
        virtual ~asio_link() {}

        virtual void setup()
        {
            link::setup();
            start();
        }

        virtual void teardown()
        {
            link::teardown();
            stop();
        }

        static boost::asio::io_service& io_service() { return io_service_; }

    protected:
        asio_link(const std::string& name) : link(name) {}

        void start();
        void stop();

        static boost::asio::io_service io_service_;
    };
}

#endif  // X2BOOST_ASIO_LINK_HPP_
