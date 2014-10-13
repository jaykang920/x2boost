// Copyright (c) 2014 Jae-jun Kang
// See the file COPYING for license details.

#ifndef X2BOOST_XPILER_DEFINITION_HPP_
#define X2BOOST_XPILER_DEFINITION_HPP_

#ifndef X2BOOST_XPILER_PRE_HPP_
#include "pre.hpp"
#endif

#include <boost/foreach.hpp>

#include "formatter.hpp"
#include "types.hpp"

namespace xpiler
{
    struct definition
    {
        definition() {}
        virtual ~definition() {}

        virtual void format(formatter_context& context) = 0;

        std::string name;
        std::string native_name;
    };

    struct consts : public definition
    {
        struct element
        {
            std::string name;
            std::string value;
            std::string native_name;
        };

        typedef boost::shared_ptr<element> element_ptr;

        consts() {}
        virtual ~consts()
        {
            BOOST_FOREACH(element* p, elements) { delete p; }
        }

        virtual void format(formatter_context& context)
        {
            context.format_consts(this);
        }

        std::string type;
        std::string native_type;
        std::vector<element*> elements;
    };

    struct cell : public definition
    {
        struct property
        {
            int index;
            std::string name;
            type_spec type_spec;
            std::string default_value;
            std::string native_name;
            std::string native_type;
        };

        typedef boost::shared_ptr<property> property_ptr;

        cell() {}
        virtual ~cell()
        {
            BOOST_FOREACH(property* p, properties) { delete p; }
        }

        virtual void format(formatter_context& context)
        {
            context.format_cell(this);
        }

        bool has_properties() { return !properties.empty(); }

        virtual bool is_event() { return false; }

        std::string base;
        std::string base_class;
        std::vector<property*> properties;
    };

    struct event : public cell
    {
        event() {}
        virtual ~event() {}

        virtual bool is_event() { return true; }

        std::string id;
    };

    struct reference
    {
        reference() {}

        std::string target;

        void format(formatter_context& context)
        {
            context.format_reference(this);
        }
    };
}

#endif  // X2BOOST_XPILER_DEFINITION_HPP_
