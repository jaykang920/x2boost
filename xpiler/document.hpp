// Copyright (c) 2014 Jae-jun Kang
// See the file COPYING for license details.

#ifndef X2BOOST_XPILER_DOCUMENT_HPP_
#define X2BOOST_XPILER_DOCUMENT_HPP_

#ifndef X2BOOST_XPILER_PRE_HPP_
#include "pre.hpp"
#endif

namespace x2boost
{
    struct Reference;
    struct Definition;

    struct Document
    {
        std::string basename;
        std::string ns;

        std::vector<boost::shared_ptr<Reference>> references;
        std::vector<boost::shared_ptr<Definition>> definitions;
    };
}

#endif  // X2BOOST_XPILER_DOCUMENT_HPP_
