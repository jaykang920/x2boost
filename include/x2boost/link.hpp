// Copyright (c) 2014 Jae-jun Kang
// See the file COPYING for license details.

#ifndef X2BOOST_LINK_HPP_
#define X2BOOST_LINK_HPP_

#ifndef X2BOOST_PRE_HPP_
#include "x2boost/pre.hpp"
#endif

#include "x2boost/case.hpp"

namespace x2
{
    // Abstract base class for concrete link cases.
    class X2BOOST_API link : public caze
    {
    public:
        explicit link(const std::string& name) : name_(name) {}

        virtual void close() = 0;

        const std::string& name() { return name_; }

        virtual void setup() {}
        virtual void teardown() {}

    protected:
        std::string name_;
    };

    // Abstract base class for concrete link sessions.
    class X2BOOST_API link_session
    {
    public:
        explicit link_session(int handle) : handle_(handle) {}

        int handle() { return handle_; }

        virtual void close() = 0;

        virtual void send(event_ptr e) = 0;
    
    private:
        int handle_;
    };
}

#endif  // X2BOOST_LINK_HPP_
