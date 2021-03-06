// Copyright (c) 2014-2017 Jae-jun Kang
// See the file LICENSE for details.

#include "boost_formatter.hpp"

#include <fstream>
#include <sstream>

#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>

#include "definition.hpp"
#include "program.hpp"
#include "types.hpp"

using namespace std;
using namespace xpiler;

namespace fs = boost::filesystem;

const char* boost_formatter_context::tab_ = "    ";

namespace
{
    std::string native_namespace;

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
        doc->basename = MixedCase2lower_case(doc->basename);
        native_namespace = doc->ns;
        boost::replace_all(native_namespace, "/", "::");

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
        cerr << e.what() << endl;
        return false;
    }
}

bool boost_formatter::is_up_to_date(const boost::filesystem::path& path, const boost::filesystem::path& out_dir)
{
    fs::path dirname = path.parent_path();
    fs::path basename = path.filename().stem();
    fs::path header_path = MixedCase2lower_case(basename.string()) + ".hpp";
    fs::path source_path = MixedCase2lower_case(basename.string()) + ".cpp";
    header_path = out_dir / header_path;
    source_path = out_dir / source_path;

    time_t source_write_time = fs::last_write_time(path);
    return (fs::exists(header_path) && fs::exists(source_path) &&
        fs::last_write_time(header_path) >= source_write_time &&
        fs::last_write_time(source_path) >= source_write_time);
}

void boost_formatter::setup()
{
    types::native_type("bool", "bool");
    types::native_type("int8", "boost::int8_t");
    types::native_type("int16", "boost::int16_t");
    types::native_type("int32", "boost::int32_t");
    types::native_type("int64", "boost::int64_t");
    types::native_type("float32", "boost::float32_t");
    types::native_type("float64", "boost::float64_t");
    types::native_type("string", "std::string");
    types::native_type("datetime", "boost::posix_time::ptime");
    types::native_type("bytes", "byte_t*");
    types::native_type("list", "std::vector");

    types::default_value("bool", "false");
    types::default_value("int8", "0");
    types::default_value("int16", "0");
    types::default_value("int32", "0");
    types::default_value("int64", "0");
    types::default_value("float32", ".0f");
    types::default_value("float64", ".0");
    types::default_value("string", "");
    types::default_value("datetime", "boost::posix_time::ptime(boost::gregorian::date(1970, 1, 1))");
    types::default_value("bytes", "NULL");
}

void boost_formatter::format_header_file(boost_formatter_context& context)
{
    ofstream out(context.target.c_str());
    if (!out)
    {
        cerr << "error opening output file: " << context.target << endl;
        return;
    }
    context.out = &out;

    string include_guard;
    if (!context.doc->ns.empty())
    {
        include_guard = context.doc->ns;
        boost::replace_all(include_guard, "/", "_");
        include_guard.append("_");
    }
    include_guard += context.doc->basename;
    include_guard += "_HPP_";
    boost::to_upper(include_guard);

    out << "// auto-generated by x2boost xpiler" << endl;
    out << endl;
    out << "#ifndef " << include_guard << endl;
    out << "#define " << include_guard << endl;

    out << endl;
    out << "#include <x2boost/x2boost.hpp>" << endl;

    if (context.doc->references.empty() == false)
    {
        out << endl;
    }
    BOOST_FOREACH(reference* ref, context.doc->references)
    {
        ref->format(context);
    }
    
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
    if (!out)
    {
        cerr << "error opening output file: " << context.target << endl;
        return;
    }
    context.out = &out;

    out << "// auto-generated by x2boost xpiler" << endl;

    if (!program::opts.precompiled_header.empty())
    {
        out << endl;
        out << "#include \"" << program::opts.precompiled_header << "\"" << endl;
    }

    out << endl;
    out << "#include \"" << context.doc->basename + ".hpp\"" << endl;
    out << endl;
    out << "#include <boost/functional/hash.hpp>" << endl;
    out << "#include <boost/thread/once.hpp>" << endl;
    out << endl;
    out << "#include <x2boost/event_factory.hpp>" << endl;
    out << "#include <x2boost/deserializer.hpp>" << endl;
    out << "#include <x2boost/serializer.hpp>" << endl;
    out << "#include <x2boost/util/string.hpp>" << endl;
    out << endl;

    if (!context.doc->ns.empty())
    {
        string ns = context.doc->ns;
        boost::replace_all(ns, "/", "::");

        out << "using namespace " << MixedCase2lower_case(ns) << ';' << endl;
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

    indent(0); *out << "class " << def->native_name << ";" << endl;
    indent(0); *out << "typedef boost::intrusive_ptr<" << def->native_name << "> "
        << def->native_name << "_ptr;" << endl;

    *out << endl;
    indent(0); *out << "class " << def->native_name << " : public " << def->base_class << endl;
    indent(0); *out << "{" << endl;

    indent(0); *out << "public:" << endl;

    for (std::size_t i = 0, count = def->properties.size(); i < count; ++i)
    {
        cell::property* prop = def->properties[i];
        bool is_primitive = types::is_primitive(prop->type.type);
        indent(1);
        if (!is_primitive) { *out << "const "; }
        *out << prop->native_type;
        if (!is_primitive) { *out << "&"; }
        *out << " " << prop->name << "() const" << endl;
        indent(1); *out << "{" << endl;
        indent(2); *out << "return " << prop->native_name << ";" << endl;
        indent(1); *out << "}" << endl;
        indent(1); *out << "void " << prop->name << "(";
        if (!is_primitive) { *out << "const "; }
        *out << prop->native_type;
        if (!is_primitive) { *out << "&"; }
        *out << " value)" << endl;
        indent(1); *out << "{" << endl;
        indent(2); *out << "fingerprint_.touch(_tag()->offset() + " << i << ");" << endl;
        indent(2); *out << prop->native_name << " = value;" << endl;
        indent(1); *out << "}" << endl;
    }
    if (!def->properties.empty())
    {
        *out << endl;
    }

    // _equals() member function
    indent(1); *out << "virtual bool _equals(const x2boost::cell& other) const;" << endl;
    // _equivalent() member function
    indent(1); *out << "virtual bool _equivalent(const x2boost::cell& other) const;" << endl;
    // _hash_code() member function
    indent(1); *out << "virtual std::size_t _hash_code(const x2boost::fingerprint& fp) const;" << endl;
    // _initialize() member function
    indent(1); *out << "void _initialize();" << endl;
    // _new() static member function
    indent(1); *out << "static " << def->native_name << "_ptr _new()" << endl;
    indent(1); *out << "{" << endl;
    indent(2); *out << "return " << def->native_name << "_ptr(new " << def->native_name << ");" << endl;
    indent(1); *out << "}" << endl;
    // _tag() static member function
    indent(1); *out << "static const tag* _tag();" << endl;
    // _type_tag() member function
    indent(1); *out << "virtual const x2boost::cell::tag* _type_tag() const;" << endl;
    *out << endl;

    // serialize/deserialize
    indent(1); *out << "virtual void _deserialize(x2boost::deserializer& deserializer);" << endl;
    indent(1); *out << "virtual int _get_encoded_length() const;" << endl;
    indent(1); *out << "virtual void _serialize(x2boost::serializer& serializer) const;" << endl;
    *out << endl;

    indent(0); *out << "protected:" << endl;

    // ctor
    indent(1); *out << def->native_name << "(std::size_t length)" << endl;
    indent(2); *out << ": " << def->base_class << "(length + _tag()->num_props())" << endl;
    indent(1); *out << "{" << endl;
    indent(2); *out << "_initialize();" << endl;
    indent(1); *out << "}" << endl;

    // _describe() member function
    indent(1); *out << "virtual void _describe(std::ostringstream& oss) const;" << endl;

    *out << endl;
    indent(0); *out << "private:" << endl;

    indent(1); *out << def->native_name << "()" << endl;
    indent(2); *out << ": " << def->base_class << "(_tag()->num_props())" << endl;
    indent(1); *out << "{" << endl;
    indent(2); *out << "_initialize();" << endl;
    indent(1); *out << "}" << endl;

    BOOST_FOREACH(cell::property* prop, def->properties)
    {
        indent(1); *out << prop->native_type << " " << prop->native_name << ";" << endl;
    }

    indent(0); *out << "};" << endl;
}

void boost_header_formatter::format_consts(consts* def)
{
    preprocess_consts(def);

    indent(0); *out << "struct " << def->native_name << endl;
    indent(0); *out << "{" << endl;
    
    BOOST_FOREACH(consts::constant* constant, def->constants)
    {
        indent(1); *out << "static const " << def->native_type << " " << constant->native_name;
        if (types::is_integer(def->type))
        {
            *out << " = " << constant->value;
        }
        *out << ";" << endl;
    }

    indent(0); *out << "};" << endl;
}

void boost_header_formatter::format_reference(reference* def)
{
    if (def->target.empty())
    {
        return;
    }
    if (def->type == file_ref_type)  // file target
    {
        string path;
        string filename = def->target;
        size_t index = filename.find_last_of('/');
        if (index != string::npos)
        {
            path = filename.substr(0, index);
            filename = filename.substr(index);
            filename = MixedCase2lower_case(filename);
            filename = path + filename;
        }
        else
        {
            filename = MixedCase2lower_case(filename);
        }
        *out << "#include \"" << filename << ".hpp\"" << endl;
    }
    else if (def->type == namespace_ref_type)  // namespace target
    {
        string ns = def->target;
        boost::replace_all(ns, "/", "::");
        *out << "using namespace " << MixedCase2lower_case(ns) << ';' << endl;
    }
}

void boost_source_formatter::format_cell(cell* def)
{
    const char* root_class = (def->is_event() ? "x2boost::event" : "x2boost::cell");

    // static type tag
    indent(0); *out << "namespace" << endl;
    indent(0); *out << "{" << endl;
    indent(1); *out << root_class << "::tag " << def->native_name << "_tag;" << endl;
    indent(1); *out << "boost::once_flag " << def->native_name << "_once = BOOST_ONCE_INIT;" << endl;
    indent(1); *out << "void " << def->native_name << "_init()" << endl;
    indent(1); *out << "{" << endl;
    indent(2); *out << def->native_name << "_tag.set("
        << def->base_class << "::_tag(), "
        << def->properties.size();
    if (def->is_event())
    {
        *out << ", " << ((event*)def)->id;
    }
    *out << ");" << endl;
    indent(1); *out << "}" << endl;
    if (def->is_event())
    {
        *out << endl;
        indent(1); *out << "struct static_" << def->native_name << "_initializer" << endl;
        indent(1); *out << "{" << endl;
        indent(2); *out << "static_" << def->native_name << "_initializer()" << endl;
        indent(2); *out << "{" << endl;
        indent(3); *out << "x2boost::event_factory::enroll(" << ((event*)def)->id << "," << endl;
        indent(4); *out << "(x2boost::event_factory::func_type)" << def->native_name << "::_new); " << endl;
        indent(2); *out << "}" << endl;
        indent(1); *out << "};" << endl;
        indent(1); *out << "static_" << def->native_name << "_initializer "
            << "static_" << def->native_name << "_init;" << endl;
    }
    indent(0); *out << "}" << endl;
    *out << endl;

    // _equals() member function
    indent(0); *out << "bool " << def->native_name << "::_equals(const x2boost::cell& other) const" << endl;
    indent(0); *out << "{" << endl;
    indent(1); *out << "if (!" << def->base_class << "::_equals(other))" << endl;
    indent(1); *out << "{" << endl;
    indent(2); *out << "return false;" << endl;
    indent(1); *out << "}" << endl;
    indent(1); *out << "const " << def->native_name << "& o = static_cast<const "
        << def->native_name << "&>(other);" << endl;
    BOOST_FOREACH(cell::property* prop, def->properties)
    {
        indent(1); *out << "if (" << prop->native_name << " != o." << prop->native_name << ")" << endl;
        indent(1); *out << "{" << endl;
        indent(2); *out << "return false;" << endl;
        indent(1); *out << "}" << endl;
    }
    indent(1); *out << "return true;" << endl;
    indent(0); *out << "}" << endl;
    *out << endl;

    // _equivalent() member function
    indent(0); *out << "bool " << def->native_name << "::_equivalent(const x2boost::cell& other) const" << endl;
    indent(0); *out << "{" << endl;
    indent(1); *out << "if (!" << def->base_class << "::_equivalent(other))" << endl;
    indent(1); *out << "{" << endl;
    indent(2); *out << "return false;" << endl;
    indent(1); *out << "}" << endl;
    indent(1); *out << "const " << def->native_name << "& o = static_cast<const "
        << def->native_name << "&>(other);" << endl;
    indent(1); *out << "x2boost::capo touched(x2boost::cell::fingerprint_, _tag()->offset());" << endl;
    for (std::size_t i = 0, count = def->properties.size(); i < count; ++i)
    {
        cell::property* prop = def->properties[i];
        indent(1); *out << "if (touched[" << i << "] && " << prop->native_name << " != o." << prop->native_name << ")" << endl;
        indent(1); *out << "{" << endl;
        indent(2); *out << "return false;" << endl;
        indent(1); *out << "}" << endl;
    }
    indent(1); *out << "return true;" << endl;
    indent(0); *out << "}" << endl;
    *out << endl;

    // _hash_code() member function
    indent(0); *out << "std::size_t " << def->native_name << "::_hash_code(const x2boost::fingerprint& fp) const" << endl;
    indent(0); *out << "{" << endl;
    indent(1); *out << "std::size_t value = " << def->base_class << "::_hash_code(fp);" << endl;
    indent(1); *out << "x2boost::capo touched(fp, _tag()->offset());" << endl;
    for (std::size_t i = 0, count = def->properties.size(); i < count; ++i)
    {
        cell::property* prop = def->properties[i];
        indent(1); *out << "if (touched[" << i << "])" << endl;
        indent(1); *out << "{" << endl;
        indent(2); *out << "boost::hash_combine(value, " << prop->native_name << ");" << endl;
        indent(1); *out << "}" << endl;
    }
    indent(1); *out << "return value;" << endl;
    indent(0); *out << "}" << endl;
    *out << endl;

    // _initialize() member function
    indent(0); *out << "void " << def->native_name << "::_initialize()" << endl;
    indent(0); *out << "{" << endl;
    BOOST_FOREACH(cell::property* prop, def->properties)
    {
        if (types::is_builtin(prop->type.type) && types::is_primitive(prop->type.type))
        {
            indent(1); *out << prop->native_name << " = " << prop->default_value << ";" << endl;
        }
    }
    indent(0); *out << "}" << endl;
    *out << endl;

    // _tag() static member function
    indent(0); *out << "const " << root_class << "::tag* " << def->native_name << "::_tag()" << endl;
    indent(0); *out << "{" << endl;
    indent(1); *out << "boost::call_once(" << def->native_name << "_init, "
        << def->native_name << "_once);" << endl;
    indent(1); *out << "return &" << def->native_name << "_tag;" << endl;
    indent(0); *out << "}" << endl;
    *out << endl;

    // _type_tag() member function
    indent(0); *out << "const x2boost::cell::tag* " << def->native_name << "::_type_tag() const" << endl;
    indent(0); *out << "{" << endl;
    indent(1); *out << "return _tag();" << endl;
    indent(0); *out << "}" << endl;

    // _describe() member function
    *out << endl;
    indent(0); *out << "void " << def->native_name << "::_describe(std::ostringstream& oss) const" << endl;
    indent(0); *out << "{" << endl;
    indent(1); *out << def->base_class << "::_describe(oss);" << endl;
    BOOST_FOREACH(cell::property* prop, def->properties)
    {
        indent(1); *out << "oss << \" " << prop->name << "=\" << " << prop->native_name << ";" << endl;
    }
    indent(0); *out << "}" << endl;

    // _deserialize()
    *out << endl;
    indent(0); *out << "void " << def->native_name << "::_deserialize(x2boost::deserializer& deserializer)" << endl;
    indent(0); *out << "{" << endl;
    indent(1); *out << def->base_class << "::_deserialize(deserializer);" << endl;
    if (def->has_properties())
    {
        indent(1); *out << "x2boost::capo touched(fingerprint_, _tag()->offset());" << endl;
        for (std::size_t i = 0, count = def->properties.size(); i < count; ++i)
        {
            cell::property* prop = def->properties[i];
            indent(1); *out << "if (touched[" << i << "])" << endl;
            indent(1); *out << "{" << endl;
            indent(2); *out << "deserializer.read(" << prop->native_name << ");" << endl;
            indent(1); *out << "}" << endl;
        }
    }
    indent(0); *out << "}" << endl;

    // _get_encoded_length()
    *out << endl;
    indent(0); *out << "int " << def->native_name << "::_get_encoded_length() const" << endl;
    indent(0); *out << "{" << endl;
    indent(1); *out << "int length = " << def->base_class << "::_get_encoded_length();" << endl;
    if (def->has_properties())
    {
        indent(1); *out << "x2boost::capo touched(fingerprint_, _tag()->offset());" << endl;
        for (std::size_t i = 0, count = def->properties.size(); i < count; ++i)
        {
            cell::property* prop = def->properties[i];
            indent(1); *out << "if (touched[" << i << "])" << endl;
            indent(1); *out << "{" << endl;
            indent(2); *out << "length += x2boost::serializer::get_encoded_length(" << prop->native_name << ");" << endl;
            indent(1); *out << "}" << endl;
        }
    }
    indent(1); *out << "return length;" << endl;
    indent(0); *out << "}" << endl;

    // _serialize()
    *out << endl;
    indent(0); *out << "void " << def->native_name << "::_serialize(x2boost::serializer& serializer) const" << endl;
    indent(0); *out << "{" << endl;
    indent(1); *out << def->base_class << "::_serialize(serializer);" << endl;
    if (def->has_properties())
    {
        indent(1); *out << "x2boost::capo touched(fingerprint_, _tag()->offset());" << endl;
        for (std::size_t i = 0, count = def->properties.size(); i < count; ++i)
        {
            cell::property* prop = def->properties[i];
            indent(1); *out << "if (touched[" << i << "])" << endl;
            indent(1); *out << "{" << endl;
            indent(2); *out << "serializer.write(" << prop->native_name << ");" << endl;
            indent(1); *out << "}" << endl;
        }
    }
    indent(0); *out << "}" << endl;
}

void boost_source_formatter::format_consts(consts* def)
{
    if (types::is_integer(def->type)) { return; }

    BOOST_FOREACH(consts::constant* constant, def->constants)
    {
        indent(0); *out << "const " << def->native_type << " " << def->native_name << "::"
            << constant->native_name << " = ";
        if (def->type == "string") { *out << "\""; }
        *out << constant->value;
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
            // Custom cell type
            return /*native_namespace + "::" + */MixedCase2lower_case(type) + "_ptr";
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
            for (std::size_t i = 0, count = type_spec.details.size(); i < count; ++i)
            {
                if (i != 0) { oss << ", "; }
                oss << format_type_spec(type_spec.details[i]);
            }
            oss << '>';
        }
        return oss.str();
    }

    void preprocess_consts(consts* def)
    {
        def->native_name = MixedCase2lower_case(def->name);
        def->native_type = types::native_type(def->type);

        BOOST_FOREACH(consts::constant* constant, def->constants)
        {
            constant->native_name = MixedCase2UPPER_CASE(constant->name);
        }
    }

    void preprocess_cell(cell* def)
    {
        def->native_name = MixedCase2lower_case(def->name);
        def->base_class = (def->base.empty() ?
            (def->is_event() ? "x2boost::event" : "x2boost::cell") :
            MixedCase2lower_case(def->base));

        if (def->is_event())
        {
            event* event_def = (event*)def;
            std::size_t index = event_def->id.find_last_of('.');
            if (index != string::npos)
            {
                string type_name = event_def->id.substr(0, index);
                string const_name = event_def->id.substr(index + 1);

                boost::replace_all(type_name, "/", "::");
                type_name = MixedCase2lower_case(type_name);
                const_name = MixedCase2UPPER_CASE(const_name);

                event_def->id = type_name + "::" + const_name;
            }
        }

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
            else
            {
                prop->default_value = types::default_value(prop->type.type);
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
                    (next != end && ::isupper(*prev) && ::isupper(*it) && ::islower(*next))/* ||
                    (!::isdigit(*prev) && ::isdigit(*it)) ||
                    (next != end && ::isdigit(*prev) && ::isdigit(*it) && !::isdigit(*next))*/)
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
