// Copyright (c) 2014-2015 Jae-jun Kang
// See the file LICENSE for details.

#ifndef X2BOOST_LINK_HPP_
#define X2BOOST_LINK_HPP_

#ifndef X2BOOST_PRE_HPP_
#include "x2boost/pre.hpp"
#endif

#include "x2boost/case.hpp"
#include "x2boost/util/ranged_int_pool.hpp"

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
        explicit link_session() : handle_(handle_pool_.acquire()) {}
        virtual ~link_session() { handle_pool_.release(handle_); }

        int handle() const { return handle_; }

        virtual void close() = 0;

        virtual void send(event_ptr e) = 0;
    
    protected:
        int handle_;

    private:
        static ranged_int_pool<1, 65536> handle_pool_;
    };
}

#endif  // X2BOOST_LINK_HPP_
