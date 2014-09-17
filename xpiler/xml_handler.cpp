// Copyright (c) 2014 Jae-jun Kang
// See the file COPYING for license details.

#include "xml_handler.hpp"

#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

using namespace std;
using namespace x2boost;

namespace pt = boost::property_tree;

bool XmlHandler::Handle(const string& path, Document** doc)
{
    *doc = NULL;

    pt::ptree p;
    pt::read_xml(path, p);

    BOOST_FOREACH(pt::ptree::value_type const& v, p.get_child("x2"))
    {
        if (v.first == "<xmlattr>")
        {
            cout << "namespace: " << v.second.get<string>("namespace") << endl;
        }
        else if (v.first == "ref")
        {

        }
        else if (v.first == "consts")
        {
            BOOST_FOREACH(pt::ptree::value_type const& v2, v.second)
            {
                if (v2.first == "<xmlattr>")
                {
                    cout << "consts name: " << v2.second.get<string>("name")
                        << " type=" << v2.second.get<string>("type", "int32") << endl;
                }
                else if (v2.first == "const")
                {
                    cout << "const name=" << v2.second.get<string>("<xmlattr>.name")
                        << " value=" << v2.second.get_value<string>() << endl;
                }
            }
        }
        else if (v.first == "cell")
        {
            BOOST_FOREACH(pt::ptree::value_type const& v2, v.second)
            {
                if (v2.first == "<xmlattr>")
                {
                    cout << "cell name: " << v2.second.get<string>("name") << endl;
                }
                else if (v2.first == "property")
                {
                    cout << "property name=" << v2.second.get<string>("<xmlattr>.name")
                        << " value=" << v2.second.get_value<string>() << endl;
                }
            }
        }
        else if (v.first == "event")
        {
            BOOST_FOREACH(pt::ptree::value_type const& v2, v.second)
            {
                if (v2.first == "<xmlattr>")
                {
                    cout << "event name: " << v2.second.get<string>("name") << endl;
                }
                else if (v2.first == "property")
                {
                    cout << "property name=" << v2.second.get<string>("<xmlattr>.name")
                        << " value=" << v2.second.get_value<string>() << endl;
                }
            }
        }
    }

    return true;
}

// EOF xml_handler.cpp
