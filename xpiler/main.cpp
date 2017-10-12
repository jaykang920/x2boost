// Copyright (c) 2014-2017 Jae-jun Kang
// See the file LICENSE for details.

#include <boost/foreach.hpp>

#include "xpiler.hpp"

int main(int argc, char* argv[])
{
    if (!xpiler::xpiler::opts.parse(argc, argv))
    {
        return 2;
    }

    xpiler::xpiler xpiler;
    BOOST_FOREACH(const std::string& path, xpiler::xpiler::opts.input)
    {
        xpiler.process(path);
    }
    return (xpiler.error ? 1 : 0);
}

// EOF main.cpp
