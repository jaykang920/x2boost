// Copyright (c) 2014-2016 Jae-jun Kang
// See the file LICENSE for details.

#ifndef X2BOOST_XPILER_XML_HANDLER_HPP_
#define X2BOOST_XPILER_XML_HANDLER_HPP_

#ifndef X2BOOST_XPILER_PRE_HPP_
#include "pre.hpp"
#endif

#include "handler.hpp"

namespace xpiler
{
    struct xml_handler : public handler
    {
        virtual bool handle(const std::string& path, document** doc);
    };
}

#endif  // X2BOOST_XPILER_XML_HANDLER_HPP_
