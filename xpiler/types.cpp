// Copyright (c) 2014-2015 Jae-jun Kang
// See the file LICENSE for details.

#include "types.hpp"

#include <sstream>

#include <boost/algorithm/string.hpp>

using namespace std;
using namespace xpiler;

types::map_type types::types_;
types::static_initializer static_init_;

string type_spec::string() const
{
    ostringstream oss;
    oss << type;
    if (!details.empty())
    {
        oss << '(';
        for (std::size_t i = 0, count = details.size(); i < count; ++i)
        {
            if (i != 0) { oss << ", "; }
            oss << details[i].string();
        }
        oss << ')';
    }
    return oss.str();
}

bool types::parse(const string& s, type_spec* spec)
{
    int index = 0;
    return parse_type_spec(s, spec, &index);
}

bool types::parse_type_spec(const string& s, type_spec* spec, int* index)
{
    string type;

    int back_margin = 0;
    int start = *index;
    int i = *index;
    for (; *index < (int)s.length(); ++(*index))
    {
        char c = s[*index];
        if (c == '(' && i < (s.length() - 1))
        {
            type = s.substr(start, *index - start);
            boost::trim(type);
            ++(*index);
            parse_details(s, &(spec->details), index);
            back_margin = 1;
            break;
        }
        else if (c == ',')
        {
            ++(*index);
            back_margin = 1;
            break;
        }
        else if (c == ')')
        {
            break;
        }
    }
    if (type.empty())
    {
        type = s.substr(start, *index - start - back_margin);
        boost::trim(type);
    }
    if (type.empty()) { return false; }
    spec->type = type;
    return true;
}

bool types::parse_details(const string& s, vector<type_spec>* details, int* index)
{
    for (; *index < s.length(); ++(*index))
    {
        char c = s[*index];
        if (c == ',') { continue; }
        if (c == ')')
        {
            ++(*index);
            break;
        }
        else
        {
            type_spec spec;
            if (parse_type_spec(s, &spec, index))
            {
                details->push_back(spec);
                --(*index);
            }
        }
    }
    return !details->empty();
}

const char* types::native_type(const std::string& type)
{
    map_type::iterator it = types_.find(type);
    return (it == types_.end() ? type.c_str() : it->second.native_type.c_str());
}

void types::set_native_type(const std::string& type, const char* native_type)
{
    map_type::iterator it = types_.find(type);
    if (it == types_.end()) { return; }
    type_traits& traits = it->second;
    traits.native_type = native_type;
}

const char* types::default_value(const std::string& type)
{
    map_type::iterator it = types_.find(type);
    return (it == types_.end() ? NULL : it->second.default_value.c_str());
}

void types::set_default_value(const std::string& type, const char* default_value)
{
    map_type::iterator it = types_.find(type);
    if (it == types_.end()) { return; }
    type_traits& traits = it->second;
    traits.default_value = default_value;
}

types::static_initializer::static_initializer()
{
    type_traits t;

    t.is_primitive = true;
    t.is_integral = false;
    
    types_.insert(map_type::value_type("bool", t));

    t.is_integral = true;

    types_.insert(map_type::value_type("int8", t));
    types_.insert(map_type::value_type("int16", t));
    types_.insert(map_type::value_type("int32", t));
    types_.insert(map_type::value_type("int64", t));

    t.is_integral = false;

    types_.insert(map_type::value_type("float32", t));
    types_.insert(map_type::value_type("float64", t));

    t.is_primitive = false;

    types_.insert(map_type::value_type("string", t));

    types_.insert(map_type::value_type("list", t));
}

// EOF types.cpp
