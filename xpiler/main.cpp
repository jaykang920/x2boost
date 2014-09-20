// Copyright (c) 2014 Jae-jun Kang
// See the file COPYING for license details.

#include <boost/foreach.hpp>

#include "options.hpp"
#include "xpiler.hpp"

int main(int argc, char* argv[])
{
    if (!xpiler::xpiler::options.parse(argc, argv))
    {
        return 2;
    }

    xpiler::xpiler xpiler;
    BOOST_FOREACH(const std::string& path, xpiler::xpiler::options.input)
    {
        xpiler.Process(path);
    }
    return (xpiler.error ? 1 : 0);
}

// EOF main.cpp
