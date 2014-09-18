// Copyright (c) 2014 Jae-jun Kang
// See the file COPYING for license details.

#ifndef X2BOOST_XPILER_BOOST_FORMATTER_HPP_
#define X2BOOST_XPILER_BOOST_FORMATTER_HPP_

#ifndef X2BOOST_XPILER_PRE_HPP_
#include "pre.hpp"
#endif

#include "formatter.hpp"

namespace xpiler
{
    struct BoostFormatter : public Formatter
    {
        virtual bool Format(Document* doc, const std::string& out_dir);

        virtual const char* GetDescription()
        {
            return "C++ with Boost";
        }

        virtual bool IsUpToDate(const std::string& path);

    private:
        void FormatHeaderFile(FormatterContext& context);
        void FormatSourceFile(FormatterContext& context);
    };

    class BoostHeaderFormatter : public FormatterContext
    {
    public:
        virtual void FormatCell(Cell* def);
        virtual void FormatConsts(Consts* def);
        virtual void FormatReference(Reference* def);
    };

    class BoostSourceFormatter : public FormatterContext
    {
    public:
        virtual void FormatCell(Cell* def);
        virtual void FormatConsts(Consts* def);
        virtual void FormatReference(Reference* def);
    };
}

#endif  // X2BOOST_XPILER_BOOST_FORMATTER_HPP_
