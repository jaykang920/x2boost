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
        ("help", "print this message and quit")
        ("path", po::value<vector<string>>(), "input path specifications");

    po::positional_options_description positional;
    positional.add("path", -1);

    po::variables_map vm;

    try
    {
        po::store(po::command_line_parser(argc, argv)
            .options(options)
            .positional(positional)
            .run(), vm);

        if (vm.count("help"))
        {
            cout << options << endl;
            return 1;
        }
        if (vm.count("path"))
        {
            input = vm["path"].as<vector<string>>();
        }

        po::notify(vm);
    }
    catch (po::error& e)
    {
        cout << "Error: " << e.what() << endl;
        return 1;
    }
	return 0;
}

// EOF options.cpp
