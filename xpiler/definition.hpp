// Copyright (c) 2014 Jae-jun Kang
// See the file COPYING for license details.

#ifndef X2BOOST_XPILER_DEFINITION_HPP_
#define X2BOOST_XPILER_DEFINITION_HPP_

#ifndef X2BOOST_XPILER_PRE_HPP_
#include "pre.hpp"
#endif

#include "formatter.hpp"

namespace xpiler
{
    struct definition
    {
        virtual void format(formatter_context& context) = 0;

        std::string name;
    };

    struct consts : public definition
    {
        struct element
        {
            std::string name;
            std::string value;
        };

        typedef boost::shared_ptr<element> element_ptr;

        virtual void format(formatter_context& context)
        {
            context.format_consts(this);
        }

        std::string type;
        std::string native_type;
        std::vector<element_ptr> elements;
    };

    struct cell : public definition
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

        virtual void format(formatter_context& context)
        {
            context.format_cell(this);
        }

        bool has_properties() { return !properties.empty(); }

        virtual bool is_event() { return false; }

        std::string base;
        std::string base_class;
        std::vector<PropertyPtr> properties;
    };

    struct event : public cell
    {
        virtual bool is_event() { return true; }

        std::string id;
    };

    struct reference
    {
        std::string target;

        void format(formatter_context& context)
        {
            context.format_reference(this);
        }
    };

    typedef boost::shared_ptr<definition> definition_ptr;
    typedef boost::shared_ptr<consts> consts_ptr;
    typedef boost::shared_ptr<cell> cell_ptr;
    typedef boost::shared_ptr<event> event_ptr;
    typedef boost::shared_ptr<reference> reference_ptr;
}

#endif  // X2BOOST_XPILER_DEFINITION_HPP_
