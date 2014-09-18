// Copyright (c) 2014 Jae-jun Kang
// See the file COPYING for license details.

#ifndef X2BOOST_XPILER_INPUT_HANDLER_HPP_
#define X2BOOST_XPILER_INPUT_HANDLER_HPP_

#ifndef X2BOOST_XPILER_PRE_HPP_
#include "pre.hpp"
#endif

namespace xpiler
{
    struct Document;

    struct Handler
    {
        virtual bool Handle(const std::string& path, Document** doc) = 0;
    };

    typedef boost::shared_ptr<Handler> HandlerPtr;
}

#endif  // X2BOOST_XPILER_INPUT_HANDLER_HPP_
