// Copyright (c) 2014 Jae-jun Kang
// See the file COPYING for license details.

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
