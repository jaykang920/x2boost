// Copyright (c) 2014 Jae-jun Kang
// See the file COPYING for license details.

#include "options.hpp"

#include <boost/program_options.hpp>

using namespace std;
using namespace x2boost;

const char* Options::kDefaultSpec = "boost";

void Options::PrintUsage()
{
    cout << "usage: xpiler (options) [path...]" << endl;
    cout << " options:" << endl;
    cout << "  -f (--force)     : force all to be re-xpiled" << endl;
    cout << "  -h (--help)      : print this message and quit" << endl;
    cout << "  -o (--out-dir)   : output root directory" << endl;
    cout << "  -r (--recursive) : process subdirectories recursively" << endl;
    cout << "  -s (--spec) spec : specifies the target formatter" << endl;
    // print available spacs
}

int Options::Parse(int argc, char* argv[])
{
    namespace po = boost::program_options;

    po::options_description options("Options");
    options.add_options()
        ("force", "force all to be re-xpiled")
        ("help", "print this message and quit")
        ("out-dir", po::value<string>(), "output root directory")
        ("recursive", "process subdirectories recursively")
        ("path", po::value<vector<string>>(), "input path specifications")
        ("spec", po::value<string>(), "specifies the target formatter");

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
            cout << options << endl;
            return 1;
        }

        if (variables.count("path"))
        {
            input = variables["path"].as<vector<string>>();
        }
        else
        {
            cout << "Error: at least one input path is required" << endl;
            return 1;
        }

        if (variables.count("force"))
        {
            forced = true;
        }
        if (variables.count("out-dir"))
        {
            outDir = variables["out-dir"].as<string>();
        }
        if (variables.count("recursive"))
        {
            recursive = true;
        }
        if (variables.count("spec"))
        {
            spec = variables["spec"].as<string>();
        }

        po::notify(variables);
    }
    catch (po::error& e)
    {
        cout << "Error: " << e.what() << endl;
        return 1;
    }
	return 0;
}

// EOF options.cpp
