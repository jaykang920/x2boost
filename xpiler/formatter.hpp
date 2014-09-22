// Copyright (c) 2014 Jae-jun Kang
// See the file COPYING for license details.

#ifndef X2BOOST_XPILER_OUTPUT_FORMATTER_HPP_
#define X2BOOST_XPILER_OUTPUT_FORMATTER_HPP_

#ifndef X2BOOST_XPILER_PRE_HPP_
#include "pre.hpp"
#endif

namespace xpiler
{
    struct cell;
    struct consts;
    struct reference;

    struct document;

    struct formatter_context
    {
        virtual void format_cell(cell* def) = 0;
        virtual void format_consts(consts* def) = 0;
        virtual void format_reference(reference* def) = 0;

        document* doc;
        std::ostream* out;
        std::string target;
    };

    struct formatter
    {
        virtual bool format(document* doc, const std::string& out_dir) = 0;

        virtual const char* description() = 0;

        virtual bool is_up_to_date(const std::string& path) = 0;
    };

    typedef boost::shared_ptr<formatter> formatter_ptr;
}

#endif  // X2BOOST_XPILER_OUTPUT_FORMATTER_HPP_
