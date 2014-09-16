// Copyright (c) 2014 Jae-jun Kang
// See the file COPYING for license details.

#include <boost/foreach.hpp>

#include "options.hpp"
#include "xpiler.hpp"

using namespace x2boost;

int main(int argc, char* argv[])
{
    if (!Xpiler::options.Parse(argc, argv))
    {
        return 2;
    }

    Xpiler xpiler;
    BOOST_FOREACH(std::string& path, Xpiler::options.input)
    {
        xpiler.Process(path);
    }
    return (xpiler.error ? 1 : 0);
}

// EOF main.cpp
