// Copyright (c) 2014 Jae-jun Kang
// See the file COPYING for license details.

#ifndef X2BOOST_XPILER_OUTPUT_FORMATTER_HPP_
#define X2BOOST_XPILER_OUTPUT_FORMATTER_HPP_

#ifndef X2BOOST_XPILER_PRE_HPP_
#include "pre.hpp"
#endif

namespace xpiler
{
    struct Cell;
    struct Consts;
    struct Reference;

    struct Document;

    struct FormatterContext
    {
        virtual void FormatCell(Cell* def) = 0;
        virtual void FormatConsts(Consts* def) = 0;
        virtual void FormatReference(Reference* def) = 0;

        Document* doc;
        std::ostream* out;
        std::string target;
    };

    struct Formatter
    {
        virtual bool Format(Document* doc, const std::string& out_dir) = 0;

        virtual const char* GetDescription() = 0;

        virtual bool IsUpToDate(const std::string& path) = 0;
    };

    typedef boost::shared_ptr<Formatter> FormatterPtr;
}

#endif  // X2BOOST_XPILER_OUTPUT_FORMATTER_HPP_
