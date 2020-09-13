// Copyright (c) 2014-2017 Jae-jun Kang
// See the file LICENSE for details.

#include "xml_handler.hpp"

#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "definition.hpp"

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
            return true;  // not an x2 definition file
		}
        document* d = *doc = new document;
        BOOST_FOREACH(pt::ptree::value_type const& v, root)
        {
            if (v.first == "<xmlattr>")
            {
                d->ns = v.second.get("namespace", "");
                boost::trim(d->ns);
            }
			else if (v.first == "references")
			{
				BOOST_FOREACH(pt::ptree::value_type const& v2, v.second)
				{
                    reference* ref = new reference;
                    ref->type = unknown_ref_type;

					if (v2.first == "namespace")
					{
                        ref->type = namespace_ref_type;
					}
					else if (v2.first == "file")
					{
                        ref->type = file_ref_type;
                    }

                    if (ref->type == unknown_ref_type)
                    {
                        delete ref;
                    }
                    else
                    {
                        ref->target = v2.second.get<string>("<xmlattr>.target");
                        boost::trim(ref->target);
                        d->references.push_back(ref);
                    }
				}
			}
			else if (v.first == "definitions")
			{
                BOOST_FOREACH(pt::ptree::value_type const& v2, v.second)
                {
                    if (v2.first == "consts")
                    {
                        consts* def = new consts;
                        BOOST_FOREACH(pt::ptree::value_type const& v3, v2.second)
                        {
                            if (v3.first == "<xmlattr>")
                            {
                                def->name = v3.second.get<string>("name");
                                def->type = v3.second.get<string>("type", "int32");
                            }
                            else if (v3.first == "const")
                            {
                                consts::constant* constant = new consts::constant;
                                constant->name = v3.second.get<string>("<xmlattr>.name");
                                constant->value = v3.second.get_value<string>();
                                boost::trim(constant->value);
                                def->constants.push_back(constant);
                            }
                        }
                        d->definitions.push_back(def);
                    }
                    else if (v2.first == "cell" || v2.first == "event")
                    {
                        bool is_event = (v2.first == "event");
                        cell* def;
                        def = (is_event ? new event : new cell);
                        BOOST_FOREACH(pt::ptree::value_type const& v3, v2.second)
                        {
                            if (v3.first == "<xmlattr>")
                            {
                                def->name = v3.second.get<string>("name");
                                def->base = v3.second.get<string>("base", "");
                                if (is_event)
                                {
                                    ((event*)def)->id = v3.second.get<string>("id");
                                }
                            }
                            else if (v3.first == "property")
                            {
                                cell::property* property = new cell::property;
                                property->name = v3.second.get<string>("<xmlattr>.name");
                                if (!types::parse(v3.second.get<string>("<xmlattr>.type"), &(property->type)))
                                {
                                    // error parsing type spec
                                    return false;
                                }
                                property->default_value = v3.second.get_value<string>();
                                boost::trim(property->default_value);
                                def->properties.push_back(property);
                            }
                        }
                        d->definitions.push_back(def);
                    }
                }
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
        cerr << e.what() << endl;
        return false;
    }
}

// EOF xml_handler.cpp
