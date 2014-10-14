// Copyright (c) 2014 Jae-jun Kang
// See the file COPYING for license details.

#include "boost_formatter.hpp"

#include <fstream>
#include <sstream>

#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>

#include "document.hpp"
#include "types.hpp"

using namespace std;
using namespace xpiler;

namespace fs = boost::filesystem;

const char* boost_formatter_context::tab_ = "    ";

namespace
{
    string format_type_spec(const type_spec& type_spec);
    string format_collection_type(const type_spec& type_spec);

    void preprocess_consts(consts* def);
    void preprocess_cell(cell* def);

    void PascalCase2camelCase(string& s);
    string MixedCase2Under_Score(const string& s);
    string MixedCase2lower_case(const string& s);
    string MixedCase2UPPER_CASE(const string& s);
}

bool boost_formatter::format(document* doc, const string& out_dir)
{
    try
    {
        fs::path basename = out_dir;
        basename /= doc->basename;

        boost_header_formatter header_context;
        header_context.doc = doc;
        header_context.target = basename.string() + ".hpp";

        boost_source_formatter source_context;
        source_context.doc = doc;
        source_context.target = basename.string() + ".cpp";

        format_header_file(header_context);
        format_source_file(source_context);

        return true;
    }
    catch (std::exception& e)
    {
        cout << e.what() << endl;
        return false;
    }
}

bool boost_formatter::is_up_to_date(const boost::filesystem::path& path)
{
    fs::path dirname = path.parent_path();
    fs::path basename = path.filename().stem();
    fs::path header_path = MixedCase2lower_case(basename.string()) + ".hpp";
    fs::path source_path = MixedCase2lower_case(basename.string()) + ".cpp";
    header_path = dirname / header_path;
    source_path = dirname / source_path;

    time_t source_write_time = fs::last_write_time(path);
    return (fs::exists(header_path) && fs::exists(source_path) &&
        fs::last_write_time(header_path) >= source_write_time &&
        fs::last_write_time(source_path) >= source_write_time);
}

void boost_formatter::setup()
{
    types::set_native_type("bool", "bool");
    types::set_native_type("int8", "boost::int8_t");
    types::set_native_type("int16", "boost::int16_t");
    types::set_native_type("int32", "boost::int32_t");
    types::set_native_type("int64", "boost::int64_t");
    types::set_native_type("float32", "boost::float32_t");
    types::set_native_type("float64", "boost::float64_t");
    types::set_native_type("string", "std::string");
    types::set_native_type("list", "std::vector");

    types::set_default_value("bool", "false");
    types::set_default_value("int8", "0");
    types::set_default_value("int16", "0");
    types::set_default_value("int32", "0");
    types::set_default_value("int64", "0");
    types::set_default_value("float32", ".0f");
    types::set_default_value("float64", ".0");
    types::set_default_value("string", "\"\"");
}

void boost_formatter::format_header_file(boost_formatter_context& context)
{
    ofstream out(context.target.c_str());
    context.out = &out;

    string include_guard;
    if (!context.doc->ns.empty())
    {
        include_guard = context.doc->ns;
        boost::replace_all(include_guard, "/", "_");
        include_guard.append("_");
    }
    include_guard += MixedCase2Under_Score(context.doc->basename);
    include_guard += "_HPP_";
    boost::to_upper(include_guard);

    out << "// auto-generated by x2boost xpiler" << endl;
    out << endl;
    out << "#ifndef " << include_guard << endl;
    out << "#define " << include_guard << endl;

    out << endl;
    out << "#include <x2boost/x2boost.hpp>" << endl;
    out << endl;

    vector<string> namespaces;
    if (!context.doc->ns.empty())
    {
        boost::split(namespaces, context.doc->ns, boost::is_any_of("/. "));
        BOOST_FOREACH(const string& ns, namespaces)
        {
            out << "namespace " << MixedCase2lower_case(ns) << " {" << endl;
        }
        out << endl;
        context.indent();
    }

    BOOST_FOREACH(definition* def, context.doc->definitions)
    {
        def->format(context);
        out << endl;
    }

    if (!context.doc->ns.empty())
    {
        context.unindent();
        BOOST_REVERSE_FOREACH(const string& ns, namespaces)
        {
            out << "}  // namespace " << MixedCase2lower_case(ns) << endl;
        }
        out << endl;
    }

    out << "#endif  // " << include_guard << endl;

    out.close();
}

void boost_formatter::format_source_file(boost_formatter_context& context)
{
    ofstream out(context.target.c_str());
    context.out = &out;

    out << "// auto-generated by x2boost xpiler" << endl;
    out << endl;
    out << "#include \"" << context.doc->basename + ".hpp\"" << endl;
    out << endl;

    if (!context.doc->ns.empty())
    {
        string ns = context.doc->ns;
        boost::replace_all(ns, "/", "::");
        boost::to_lower(ns);

        out << "using namespace " << ns << ';' << endl;
        out << endl;
    }

    BOOST_FOREACH(definition* def, context.doc->definitions)
    {
        def->format(context);
        out << endl;
    }

    out << "// end of " << fs::path(context.target).filename().string() << endl;

    out.close();
}

void boost_formatter_context::indent(int level) const
{
    int indentation = base_indentation_ + level;
    for (int i = 0; i < indentation; ++i)
    {
        *out << tab_;
    }
}

void boost_header_formatter::format_cell(cell* def)
{
    preprocess_cell(def);

    indent(0); *out << "class " << def->native_name << " : public " << def->base_class << endl;

    indent(0); *out << "public:" << endl;

    indent(1); *out << "static " << def->native_name << "_ptr _new()" << endl;
    indent(1); *out << "{" << endl;
    indent(2); *out << "return " << def->native_name << "_ptr(new " << def->native_name << ");" << endl;
    indent(1); *out << "}" << endl;

    *out << endl;
    indent(1); *out << "void _initialize();" << endl;

    *out << endl;

    BOOST_FOREACH(cell::property* prop, def->properties)
    {
        bool is_primitive = types::is_primitive(prop->type.type);
        indent(1);
        if (!is_primitive) { *out << "const "; }
        *out << prop->native_type;
        if (!is_primitive) { *out << "&"; }
        *out << " " << prop->name << "() const" << endl;
        indent(1); *out << "{" << endl;
        indent(2); *out << "return " << prop->native_name << ";" << endl;
        indent(1); *out << "}" << endl;
        indent(1); *out << "void set_" << prop->name << "(";
        if (!is_primitive) { *out << "const "; }
        *out << prop->native_type;
        if (!is_primitive) { *out << "&"; }
        *out << " value)" << endl;
        indent(1); *out << "{" << endl;
        indent(2); *out << prop->native_name << " = value;" << endl;
        indent(1); *out << "}" << endl;
    }
    if (!def->properties.empty())
    {
        *out << endl;
    }

    indent(0); *out << "protected:" << endl;

    // ctor
    indent(1); *out << def->native_name << "(std::size_t length)" << endl;
    indent(2); *out << ": " << def->base_class << "(length + _tag()->num_props())" << endl;
    indent(1); *out << "{" << endl;
    indent(2); *out << "initialize();" << endl;
    indent(1); *out << "}" << endl;

    indent(1); *out << "void _describe(std::ostringstream& oss) const;" << endl;

    *out << endl;
    indent(0); *out << "private:" << endl;

    indent(1); *out << def->native_name << "()" << endl;
    indent(2); *out << ": " << def->base_class << "(_tag()->num_props())" << endl;
    indent(1); *out << "{" << endl;
    indent(2); *out << "initialize();" << endl;
    indent(1); *out << "}" << endl;

    BOOST_FOREACH(cell::property* prop, def->properties)
    {
        indent(1); *out << prop->native_type << " " << prop->native_name << ";" << endl;
    }

    indent(0); *out << "};" << endl;

    indent(0); *out << "typedef boost::shared_ptr<" << def->native_name << "> "
        << def->native_name << "_ptr;" << endl;
}

void boost_header_formatter::format_consts(consts* def)
{
    preprocess_consts(def);

    indent(0); *out << "struct " << def->native_name << endl;
    indent(0); *out << "{" << endl;
    
    BOOST_FOREACH(consts::element* elem, def->elements)
    {
        indent(1); *out << "const " << def->native_type << " " << elem->native_name;
        if (types::is_integral(def->type))
        {
            *out << " = " << elem->value;
        }
        *out << ";" << endl;
    }

    indent(0); *out << "}" << endl;
}

void boost_header_formatter::format_reference(reference* def)
{

}

void boost_source_formatter::format_cell(cell* def)
{
    indent(0); *out << "void " << def->native_name << "::_describe(std::ostringstream& oss) const" << endl;
    indent(0); *out << "{" << endl;
    indent(1); *out << def->base_class << "::describe(oss);" << endl;
    BOOST_FOREACH(cell::property* prop, def->properties)
    {
        indent(1); *out << "oss << \" " << prop->name << "=\" << " << prop->native_name << ";" << endl;
    }
    indent(0); *out << "}" << endl;
    *out << endl;

    indent(0); *out << "void " << def->native_name << "::_initialize()" << endl;
    indent(0); *out << "{" << endl;
    BOOST_FOREACH(cell::property* prop, def->properties)
    {
        indent(1); *out << prop->native_name << " = " << prop->default_value << ";" << endl;
    }
    indent(0); *out << "}" << endl;
    //*out << endl;
}

void boost_source_formatter::format_consts(consts* def)
{
    if (types::is_integral(def->type)) { return; }

    BOOST_FOREACH(consts::element* elem, def->elements)
    {
        indent(0); *out << def->native_type << " " << def->native_name << "::"
            << elem->native_name << " = ";
        if (def->type == "string") { *out << "\""; }
        *out << elem->value;
        if (def->type == "string") { *out << "\""; }
        else if (def->type == "float32") { *out << "f"; }
        *out << ";" << endl;
    }
}

void boost_source_formatter::format_reference(reference* def)
{

}

namespace
{
    string format_type_spec(const type_spec& type_spec)
    {
        const string& type = type_spec.type;
        if (!types::is_builtin(type))
        {
            return type;  // custom type
        }
        return types::is_primitive(type) ? types::native_type(type)
            : format_collection_type(type_spec);
    }

    string format_collection_type(const type_spec& type_spec)
    {
        ostringstream oss;
        oss << types::native_type(type_spec.type);
        if (!type_spec.details.empty())
        {
            oss << '<';
            for (size_t i = 0, count = type_spec.details.size(); i < count; ++i)
            {
                if (i != 0) { oss << ", "; }
                oss << format_type_spec(type_spec.details[i]);
            }
        }
        return oss.str();
    }

    void preprocess_consts(consts* def)
    {
        def->native_name = MixedCase2lower_case(def->name);
        def->native_type = types::native_type(def->type);

        BOOST_FOREACH(consts::element* elem, def->elements)
        {
            elem->native_name = MixedCase2lower_case(elem->name);
        }
    }

    void preprocess_cell(cell* def)
    {
        def->native_name = MixedCase2lower_case(def->name);
        def->base_class = (def->base.empty() ?
            (def->is_event() ? "x2::event" : "x2::cell") :
            MixedCase2lower_case(def->base_class));

        int index = 0;
        BOOST_FOREACH(cell::property* prop, def->properties)
        {
            prop->index = index++;
            prop->name = MixedCase2lower_case(prop->name);
            prop->native_name = prop->name + "_";
            prop->native_type = format_type_spec(prop->type);
            
            // default value
            if (types::is_primitive(prop->type.type))
            {
                if (prop->default_value.empty())
                {
                    prop->default_value = types::default_value(prop->type.type);
                }
            }
            else if (prop->type.type == "string")
            {
                prop->default_value = "\"" + prop->default_value + "\"";
            }
        }
    }

    void PascalCase2camelCase(string& s) {
        if (s.empty() || !::isupper(s[0]))
        {
            return;
        }
        string::iterator it = s.begin(), end = s.end(), next;
        *it++ = (char)::tolower(*it);
        next = it;
        if (next != end) { ++next; }
        while (it != end)
        {
            if (!::isupper(*it) || (next != end && !::isupper(*next)))
            {
                break;
            }
            *it++ = (char)::tolower(*it);
            if (next != end) { ++next; }
        }
    }

    string MixedCase2Under_Score(const string& s)
    {
        string result;
        if (!s.empty())
        {
            back_insert_iterator<string> out = back_inserter(result);
            string::const_iterator it = s.begin(), end = s.end(), prev, next;
            prev = it;
            *out++ = *it++;
            next = it;
            if (next != end) { ++next; }
            while (it != end)
            {
                if ((::islower(*prev) && ::isupper(*it)) ||
                    (next != end && ::isupper(*prev) && ::isupper(*it) && ::islower(*next)) ||
                    (!::isdigit(*prev) && ::isdigit(*it)) ||
                    (next != end && ::isdigit(*prev) && ::isdigit(*it) && !::isdigit(*next)))
                {
                    *out++ = '_';
                }
                *out++ = *it++;
                ++prev;
                if (next != end) { ++next; }
            }
        }
        return result;
    }

    string MixedCase2lower_case(const string& s)
    {
        string result = MixedCase2Under_Score(s);
        boost::to_lower(result);
        return result;
    }

    string MixedCase2UPPER_CASE(const string& s)
    {
        string result = MixedCase2Under_Score(s);
        boost::to_upper(result);
        return result;
    }

}  // namespace

// EOF boost_formatter.cpp
