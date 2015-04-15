// Copyright (c) 2014-2015 Jae-jun Kang
// See the file LICENSE for details.

#ifndef X2BOOST_XPILER_BOOST_FORMATTER_HPP_
#define X2BOOST_XPILER_BOOST_FORMATTER_HPP_

#ifndef X2BOOST_XPILER_PRE_HPP_
#include "pre.hpp"
#endif

#include <map>

#include "formatter.hpp"

namespace xpiler
{
    class boost_formatter_context;

    struct boost_formatter : public formatter
    {
        virtual bool format(document* doc, const std::string& out_dir);

        virtual bool is_up_to_date(const boost::filesystem::path& path);

        virtual void setup();

        virtual const char* description()
        {
            return "C++ with Boost";
        }

    private:
        void format_header_file(boost_formatter_context& context);
        void format_source_file(boost_formatter_context& context);
    };

    class boost_formatter_context : public formatter_context
    {
    public:
        void indent() { ++base_indentation_; }
        void unindent() { --base_indentation_; }

    protected:
        boost_formatter_context() : base_indentation_(0) {}

        void indent(int level) const;

        static const char* tab_;

        int base_indentation_;
    };

    class boost_header_formatter : public boost_formatter_context
    {
    public:
        virtual void format_cell(cell* def);
        virtual void format_consts(consts* def);
        virtual void format_reference(reference* def);
    };

    class boost_source_formatter : public boost_formatter_context
    {
    public:
        virtual void format_cell(cell* def);
        virtual void format_consts(consts* def);
        virtual void format_reference(reference* def);
    };
}

#endif  // X2BOOST_XPILER_BOOST_FORMATTER_HPP_
