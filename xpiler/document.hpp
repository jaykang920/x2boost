// Copyright (c) 2014 Jae-jun Kang
// See the file COPYING for license details.

#ifndef X2BOOST_XPILER_DOCUMENT_HPP_
#define X2BOOST_XPILER_DOCUMENT_HPP_

#ifndef X2BOOST_XPILER_PRE_HPP_
#include "pre.hpp"
#endif

#include <boost/foreach.hpp>

namespace xpiler
{
    struct reference;
    struct definition;

    struct document
    {
        document() {}
        ~document()
        {
            BOOST_FOREACH(reference* p, references) { delete p; }
            BOOST_FOREACH(definition* p, definitions) { delete p; }
        }

        std::string basename;
        std::string ns;

        std::vector<reference*> references;
        std::vector<definition*> definitions;
    };
}

#endif  // X2BOOST_XPILER_DOCUMENT_HPP_
