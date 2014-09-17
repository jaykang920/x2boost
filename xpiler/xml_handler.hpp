// Copyright (c) 2014 Jae-jun Kang
// See the file COPYING for license details.

#ifndef X2BOOST_XPILER_XML_HANDLER_HPP_
#define X2BOOST_XPILER_XML_HANDLER_HPP_

#ifndef X2BOOST_XPILER_PRE_HPP_
#include "pre.hpp"
#endif

#include "input_handler.hpp"

namespace x2boost
{
    struct XmlHandler : public InputHandler
    {
        virtual bool Handle(const std::string& path, Document** doc);
    };
}

#endif  // X2BOOST_XPILER_XML_HANDLER_HPP_
