// Copyright (c) 2014-2015 Jae-jun Kang
// See the file LICENSE for details.

#ifndef X2BOOST_ARRAY_SEGMENT_HPP_
#define X2BOOST_ARRAY_SEGMENT_HPP_

#ifndef X2BOOST_PRE_HPP_
#include "x2boost/pre.hpp"
#endif

namespace x2
{
    template<typename T>
    struct X2BOOST_API array_segment
    {
        deleter() {}

        T* base;
        std::size_t offset;
        std::size_t length;
    };
}

#endif  // X2BOOST_ARRAY_SEGMENT_HPP_
