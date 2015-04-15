// Copyright (c) 2014-2015 Jae-jun Kang
// See the file LICENSE for details.

#ifndef X2BOOST_DELETER_HPP_
#define X2BOOST_DELETER_HPP_

#ifndef X2BOOST_PRE_HPP_
#include "x2boost/pre.hpp"
#endif

namespace x2
{
    struct X2BOOST_API deleter
    {
        deleter() {}

        template<typename T>
        void operator()(T* p) { delete p; }
    };

    struct X2BOOST_API null_deleter
    {
        null_deleter() {}

        template<typename T>
        void operator()(T* /*p*/) {}
    };
}

#endif  // X2BOOST_DELETER_HPP_
