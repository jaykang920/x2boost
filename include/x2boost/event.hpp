// Copyright (c) 2014 Jae-jun Kang
// See the file COPYING for license details.

#ifndef X2BOOST_EVENT_HPP_
#define X2BOOST_EVENT_HPP_

#ifndef X2BOOST_PRE_HPP_
#include "x2boost/pre.hpp"
#endif

#include "x2boost/cell.hpp"

namespace x2
{
    class X2BOOST_API event : public cell
    {
    public:
        virtual ~event() {}

        const char* _channel() const
        {
            return _channel_;
        }

        event& _set_channel(const char* value)
        {
            _channel_ = value;
        }

    private:
        const char* _channel_;
    };
}

#endif  // X2BOOST_EVENT_HPP_
