// Copyright (c) 2014-2016 Jae-jun Kang
// See the file LICENSE for details.

#ifndef X2BOOST_XPILER_TYPES_HPP_
#define X2BOOST_XPILER_TYPES_HPP_

#ifndef X2BOOST_XPILER_PRE_HPP_
#include "pre.hpp"
#endif

#include <map>

namespace xpiler
{
    struct type_spec
    {
        type_spec() {}

        std::string string() const;

        std::string type;
        std::vector<type_spec> details;
    };

    struct type_traits
    {
        type_traits() {}

        bool is_integral;
        bool is_primitive;

        std::string native_type;
        std::string default_value;
    };

    class types
    {
    public:
        static bool is_builtin(const std::string& type)
        {
            return (types_.count(type) != 0);
        }

        static bool is_integral(const std::string& type)
        {
            map_type::iterator it = types_.find(type);
            if (it == types_.end()) { return false; }
            return it->second.is_integral;
        }

        static bool is_primitive(const std::string& type)
        {
            map_type::iterator it = types_.find(type);
            if (it == types_.end()) { return false; }
            return it->second.is_primitive;
        }

        static bool parse(const std::string& s, type_spec* spec);
        static bool parse_type_spec(const std::string& s, type_spec* spec, int* index);
        static bool parse_details(const std::string& s, std::vector<type_spec>* details, int* index);

        static const char* native_type(const std::string& type);
        static void set_native_type(const std::string& type, const char* native_type);

        static const char* default_value(const std::string& type);
        static void set_default_value(const std::string& type, const char* default_value);

        struct static_initializer
        {
            static_initializer();
        };

    private:
        typedef std::map<std::string, type_traits> map_type;

        types() {}

        static map_type types_;

        static static_initializer static_init_;
    };
}

#endif  // X2BOOST_XPILER_TYPES_HPP_
