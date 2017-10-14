// Copyright (c) 2014-2017 Jae-jun Kang
// See the file LICENSE for details.

#ifndef X2BOOST_QUEUE_HPP_
#define X2BOOST_QUEUE_HPP_

#ifndef X2BOOST_PRE_HPP_
#include "x2boost/pre.hpp"
#endif

namespace x2boost
{
    /// Abstract base class for blocking event queue implementations.
    struct X2BOOST_API event_queue
    {
        virtual ~event_queue() {}

        virtual void close() = 0;

        virtual event_ptr dequeue() = 0;

        virtual bool enqueue(const event_ptr& value) = 0;

        virtual bool try_dequeue(event_ptr* value) = 0;

        virtual size_t size() const = 0;
    };
}

#endif  // X2BOOST_QUEUE_HPP_
