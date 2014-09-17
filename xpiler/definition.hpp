// Copyright (c) 2014 Jae-jun Kang
// See the file COPYING for license details.

#ifndef X2BOOST_XPILER_DEFINITION_HPP_
#define X2BOOST_XPILER_DEFINITION_HPP_

#ifndef X2BOOST_XPILER_PRE_HPP_
#include "pre.hpp"
#endif

#include "output_formatter.hpp"

namespace x2boost
{
    struct Definition
    {
        virtual void Format(FormatterContext& context) = 0;
    };

    struct Reference : Definition
    {
        std::string target;

        virtual void Format(FormatterContext& context)
        {
            context.FormatRefrence(this);
        }
    };

    struct Consts : Definition
    {
        struct Const
        {
            std::string name;
            std::string value;
        };

        virtual void Format(FormatterContext& context)
        {
            context.FormatConsts(this);
        }

        std::string type;
        std::string native_type;
        std::vector<Const> constants;
    };

    struct Cell : Definition
    {
        struct Property
        {
            int index;
            std::string name;
            //TypeSpec type_spec;
            std::string default_value;
            std::string native_name;
            std::string native_type;
        };

        virtual void Format(FormatterContext& context)
        {
            context.FormatCell(this);
        }

        bool HasProperties() { return !properties.empty(); }

        virtual bool IsEvent() { return false; }

        std::string base;
        std::string base_class;
        std::vector<Property> properties;
    };

    struct Event : Cell
    {
        virtual bool IsEvent() { return true; }

        std::string id;
    }
}

#endif  // X2BOOST_XPILER_DEFINITION_HPP_
