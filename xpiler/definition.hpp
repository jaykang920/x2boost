// Copyright (c) 2014-2017 Jae-jun Kang
// See the file LICENSE for details.

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
    enum reference_type
    {
        unknown_ref_type,
        namespace_ref_type,
        file_ref_type
    };

    /// Represents a reference specification.
	struct reference
	{
        reference() {}

        reference_type type;
		std::string target;

		void format(formatter_context& context)
		{
			context.format_reference(this);
		}
    };

	/// Represents an abstract definition.
    struct definition
    {
        definition() {}
        virtual ~definition() {}

        virtual void format(formatter_context& context) = 0;

        std::string name;
        std::string native_name;
    };

	/// Represents a set of constants.
    struct consts : public definition
    {
        struct constant
        {
            std::string name;
            std::string value;
            std::string native_name;
        };

        consts() {}
        virtual ~consts()
        {
            BOOST_FOREACH(constant* p, constants) { delete p; }
        }

        virtual void format(formatter_context& context)
        {
            context.format_consts(this);
        }

        std::string type;
        std::string native_type;
        std::vector<constant*> constants;
    };

	/// Represents a cell definition.
    struct cell : public definition
    {
        struct property
        {
            int index;
            std::string name;
            type_spec type;
            std::string default_value;
            std::string native_name;
            std::string native_type;
        };

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

	/// Represents an event definition.
    struct event : public cell
    {
        event() {}
        virtual ~event() {}

        virtual bool is_event() { return true; }

        std::string id;
    };

	/// Represents a single definition document.
	struct document
	{
		document() {}
		~document()
		{
			BOOST_FOREACH(reference* p, references) { delete p; }
			BOOST_FOREACH(definition* p, definitions) { delete p; }
		}

		std::string basename;
		std::string ns;         // namespace specified in the definition file

		std::vector<reference*> references;
		std::vector<definition*> definitions;
	};
}

#endif  // X2BOOST_XPILER_DEFINITION_HPP_
