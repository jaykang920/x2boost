// Copyright (c) 2014 Jae-jun Kang
// See the file COPYING for license details.

#include "options.hpp"

#include <boost/program_options/option.hpp>

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
	return 0;
}

// EOF options.cpp
