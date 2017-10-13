// Copyright (c) 2014-2017 Jae-jun Kang
// See the file LICENSE for details.

#include <boost/foreach.hpp>

#include "program.hpp"

using namespace std;
using namespace xpiler;

int main(int argc, char* argv[])
{
    if (!program::opts.parse(argc, argv))
    {
        return 2;
    }

    program program;
    BOOST_FOREACH(const string& path, program::opts.input)
    {
		program.process(path);
    }
	return (program.error ? 1 : 0);
}

// EOF main.cpp
