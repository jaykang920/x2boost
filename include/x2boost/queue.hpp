// Copyright (c) 2014-2017 Jae-jun Kang
// See the file LICENSE for details.

#ifndef X2BOOST_QUEUE_HPP_
#define X2BOOST_QUEUE_HPP_

#ifndef X2BOOST_PRE_HPP_
#include "x2boost/pre.hpp"
#endif

namespace x2boost
{
    template<typename T>
    struct X2BOOST_API queue_interface
    {
        virtual ~queue_interface() {}

        virtual void close() = 0;

        virtual T dequeue() = 0;

        virtual bool enqueue(T value) = 0;

        virtual bool try_dequeue(T* value) = 0;

        virtual size_t size() const = 0;
    };
}

#endif  // X2BOOST_QUEUE_HPP_
