// Copyright (c) 2014 Jae-jun Kang
// See the file COPYING for license details.

#ifndef X2BOOST_XPILER_INPUT_HANDLER_HPP_
#define X2BOOST_XPILER_INPUT_HANDLER_HPP_

#ifndef X2BOOST_XPILER_PRE_HPP_
#include "pre.hpp"
#endif

namespace x2boost
{
    struct Document;

    struct InputHandler
    {
        virtual bool Handle(const std::string& path, Document** doc) = 0;
    };
}

#endif  // X2BOOST_XPILER_INPUT_HANDLER_HPP_
