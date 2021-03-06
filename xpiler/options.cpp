// Copyright (c) 2014-2017 Jae-jun Kang
// See the file LICENSE for details.

#include "options.hpp"

#include <iomanip>

#include <boost/foreach.hpp>
#include <boost/program_options.hpp>

#include "formatter.hpp"
#include "program.hpp"

using namespace std;
using namespace xpiler;

const char* options::default_spec = "boost";

bool options::parse(int argc, char* argv[])
{
    namespace po = boost::program_options;

    po::options_description options("options");
    options.add_options()
        ("force", "force all to be re-xpiled")
        ("help", "print this message and quit")
        ("out-dir", po::value<string>(), "output root directory")
        ("recursive", "process subdirectories recursively")
        ("path", po::value<vector<string> >(), "input path specifications")
        ("spec", po::value<string>(), "specifies the target formatter")
        ("pch", po::value<string>(), "specifies pre-compiled header file name");

    po::positional_options_description positional;
    positional.add("path", -1);

    try
    {
        po::variables_map variables;
        po::store(po::command_line_parser(argc, argv)
            .options(options)
            .positional(positional)
            .run(), variables);

        if (variables.count("help"))
        {
            cout << "usage: xpiler [options] path..." << endl;
            cout << options;
            cout << "specs:" << endl;
            BOOST_FOREACH(const program::formatter_map_type::value_type& pair,
                program::formatters())
            {
                cout << setw(21) << pair.first << " : "
                    << pair.second->description();
                if (pair.first == default_spec)
                {
                    cout << " (default)";
                }
                cout << endl;
            }
            return false;
        }

        if (variables.count("path"))
        {
            input = variables["path"].as<vector<string> >();
        }
        else
        {
            cerr << "error: at least one input path required" << endl;
            return false;
        }

        if (variables.count("force"))
        {
            forced = true;
        }
        if (variables.count("out-dir"))
        {
            out_dir = variables["out-dir"].as<string>();
        }
        if (variables.count("recursive"))
        {
            recursive = true;
        }
        if (variables.count("spec"))
        {
            spec = variables["spec"].as<string>();
            const program::formatter_map_type& formatters = program::formatters();
            program::formatter_map_type::const_iterator it = formatters.find(spec);
            if (it == formatters.end())
            {
                cerr << "error: unknown target formatter specified: "
                    << spec << endl;
                return false;
            }
        }
        if (variables.count("pch"))
        {
            precompiled_header = variables["pch"].as<string>();
        }

        po::notify(variables);
        return true;
    }
    catch (po::error& e)
    {
        cerr << "error: " << e.what() << endl;
        return false;
    }
}

// EOF options.cpp
