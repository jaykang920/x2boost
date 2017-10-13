// Copyright (c) 2014-2017 Jae-jun Kang
// See the file LICENSE for details.

#ifndef X2BOOST_XPILER_FORMATTER_HPP_
#define X2BOOST_XPILER_FORMATTER_HPP_

#ifndef X2BOOST_XPILER_PRE_HPP_
#include "pre.hpp"
#endif

#include <boost/filesystem.hpp>

namespace xpiler
{
    struct cell;
    struct consts;
	struct document;
	struct reference;

    /// Abstract base class for output file formatters.
    struct formatter
    {
        virtual bool format(document* doc, const std::string& out_dir) = 0;

        virtual bool is_up_to_date(const boost::filesystem::path& path) = 0;

        virtual void setup() = 0;

        virtual const char* description() = 0;
    };

    /// Abstract base class for concrete formatter contexts.
    struct formatter_context
    {
        virtual void format_cell(cell* def) = 0;
        virtual void format_consts(consts* def) = 0;
        virtual void format_reference(reference* def) = 0;

        document* doc;
        std::ostream* out;
        std::string target;
    };
}

#endif  // X2BOOST_XPILER_FORMATTER_HPP_
