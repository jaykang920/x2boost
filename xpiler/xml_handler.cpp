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

bool XmlHandler::Handle(const string& path, Document** doc)
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
        Document* document = *doc = new Document;
        BOOST_FOREACH(pt::ptree::value_type const& v, root)
        {
            if (v.first == "<xmlattr>")
            {
                document->ns = v.second.get<string>("namespace");
            }
            else if (v.first == "ref")
            {
                ReferencePtr ref(new Reference);
                ref->target = v.second.get<string>("<xmlattr>.target");
                document->references.push_back(ref);
            }
            else if (v.first == "consts")
            {
                ConstsPtr def(new Consts);
                BOOST_FOREACH(pt::ptree::value_type const& v2, v.second)
                {
                    if (v2.first == "<xmlattr>")
                    {
                        def->name = v2.second.get<string>("name");
                        def->type = v2.second.get<string>("type", "int32");
                    }
                    else if (v2.first == "const")
                    {
                        Consts::ConstPtr constant(new Consts::Const);
                        constant->name = v2.second.get<string>("<xmlattr>.name");
                        constant->value = v2.second.get_value<string>();
                        boost::trim(constant->value);
                        def->constants.push_back(constant);
                    }
                }
                document->definitions.push_back(def);
            }
            else if (v.first == "cell" || v.first == "event")
            {
                bool is_event = (v.first == "event");
                CellPtr def;
                def.reset(is_event ? new Event : new Cell);
                BOOST_FOREACH(pt::ptree::value_type const& v2, v.second)
                {
                    if (v2.first == "<xmlattr>")
                    {
                        def->name = v2.second.get<string>("name");
                        if (is_event)
                        {
                            ((Event*)def.get())->id = v2.second.get<string>("id");
                        }
                    }
                    else if (v2.first == "property")
                    {
                        Cell::PropertyPtr property(new Cell::Property);
                        property->name = v2.second.get<string>("<xmlattr>.name");
                        //property->type = v2.second.get<string>("<xmlattr>.type");
                        property->default_value = v2.second.get_value<string>();
                        boost::trim(property->default_value);
                        def->properties.push_back(property);
                    }
                }
                document->definitions.push_back(def);
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
