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
    struct boost_formatter : public formatter
    {
        virtual bool format(document* doc, const std::string& out_dir);

        virtual const char* description()
        {
            return "C++ with Boost";
        }

        virtual bool is_up_to_date(const std::string& path);

    private:
        void format_header_file(formatter_context& context);
        void format_source_file(formatter_context& context);
    };

    class BoostHeaderformatter : public formatter_context
    {
    public:
        virtual void format_cell(cell* def);
        virtual void format_consts(consts* def);
        virtual void format_reference(reference* def);
    };

    class BoostSourceformatter : public formatter_context
    {
    public:
        virtual void format_cell(cell* def);
        virtual void format_consts(consts* def);
        virtual void format_reference(reference* def);
    };
}

#endif  // X2BOOST_XPILER_BOOST_FORMATTER_HPP_
