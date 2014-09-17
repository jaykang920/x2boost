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

        std::string name;
    };

    struct Consts : Definition
    {
        struct Const
        {
            std::string name;
            std::string value;
        };

        typedef boost::shared_ptr<Const> ConstPtr;

        virtual void Format(FormatterContext& context)
        {
            context.FormatConsts(this);
        }

        std::string type;
        std::string native_type;
        std::vector<ConstPtr> constants;
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

        typedef boost::shared_ptr<Property> PropertyPtr;

        virtual void Format(FormatterContext& context)
        {
            context.FormatCell(this);
        }

        bool HasProperties() { return !properties.empty(); }

        virtual bool IsEvent() { return false; }

        std::string base;
        std::string base_class;
        std::vector<PropertyPtr> properties;
    };

    struct Event : Cell
    {
        virtual bool IsEvent() { return true; }

        std::string id;
    };

    struct Reference
    {
        std::string target;

        void Format(FormatterContext& context)
        {
            context.FormatReference(this);
        }
    };

    typedef boost::shared_ptr<Definition> DefinitionPtr;
    typedef boost::shared_ptr<Consts> ConstsPtr;
    typedef boost::shared_ptr<Cell> CellPtr;
    typedef boost::shared_ptr<Event> EventPtr;
    typedef boost::shared_ptr<Reference> ReferencePtr;
}

#endif  // X2BOOST_XPILER_DEFINITION_HPP_
