// Copyright (c) 2014 Jae-jun Kang
// See the file COPYING for license details.

#include "xml_handler.hpp"

#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "definition.hpp"
#include "document.hpp"

using namespace std;
using namespace xpiler;

namespace pt = boost::property_tree;

bool xml_handler::handle(const string& path, document** doc)
{
    *doc = NULL;
    try
    {
        pt::ptree p;
        pt::read_xml(path, p);

        pt::ptree& root = p.get_child("x2");
        if (root.empty())
        {
            return true;
        }
        document* d = *doc = new document;
        BOOST_FOREACH(pt::ptree::value_type const& v, root)
        {
            if (v.first == "<xmlattr>")
            {
                d->ns = v.second.get("namespace", "");
                boost::trim(d->ns);
            }
            else if (v.first == "ref")
            {
                reference* ref = new reference;
                ref->target = v.second.get<string>("<xmlattr>.target");
                d->references.push_back(ref);
            }
            else if (v.first == "consts")
            {
                consts* def = new consts;
                BOOST_FOREACH(pt::ptree::value_type const& v2, v.second)
                {
                    if (v2.first == "<xmlattr>")
                    {
                        def->name = v2.second.get<string>("name");
                        def->type = v2.second.get<string>("type", "int32");
                    }
                    else if (v2.first == "const")
                    {
                        consts::element* element = new consts::element;
                        element->name = v2.second.get<string>("<xmlattr>.name");
                        element->value = v2.second.get_value<string>();
                        boost::trim(element->value);
                        def->elements.push_back(element);
                    }
                }
                d->definitions.push_back(def);
            }
            else if (v.first == "cell" || v.first == "event")
            {
                bool is_event = (v.first == "event");
                cell* def;
                def = (is_event ? new event : new cell);
                BOOST_FOREACH(pt::ptree::value_type const& v2, v.second)
                {
                    if (v2.first == "<xmlattr>")
                    {
                        def->name = v2.second.get<string>("name");
                        def->base = v2.second.get("extends", "");
                        if (is_event)
                        {
                            ((event*)def)->id = v2.second.get<string>("id");
                        }
                    }
                    else if (v2.first == "property")
                    {
                        cell::property* property = new cell::property;
                        property->name = v2.second.get<string>("<xmlattr>.name");
                        if (!types::parse(v2.second.get<string>("<xmlattr>.type"), &(property->type)))
                        {
                            // error parsing type spec
                            return false;
                        }
                        property->default_value = v2.second.get_value<string>();
                        boost::trim(property->default_value);
                        def->properties.push_back(property);
                    }
                }
                d->definitions.push_back(def);
            }
        }
        return true;
    }
    catch (std::exception& e)
    {
        if (*doc != NULL)
        {
            delete *doc;
            *doc = NULL;
        }
        cout << e.what() << endl;
        return false;
    }
}

// EOF xml_handler.cpp
