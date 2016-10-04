// Copyright (c) 2014-2016 Jae-jun Kang
// See the file LICENSE for details.

#ifndef X2BOOST_XPILER_HANDLER_HPP_
#define X2BOOST_XPILER_HANDLER_HPP_

#ifndef X2BOOST_XPILER_PRE_HPP_
#include "pre.hpp"
#endif

namespace xpiler
{
    struct document;

    struct handler
    {
        virtual bool handle(const std::string& path, document** doc) = 0;
    };
}

#endif  // X2BOOST_XPILER_HANDLER_HPP_
