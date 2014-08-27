// Copyright (c) 2014 Jae-jun Kang
// See the file COPYING for license details.

#ifndef X2BOOST_XPILER_OPTIONS_HPP_
#define X2BOOST_XPILER_OPTIONS_HPP_

#ifndef X2BOOST_XPILER_PRE_HPP_
#include "pre.hpp"
#endif

namespace x2boost
{
    struct Options
    {
        Options()
          : forced(false),
            recursive(false),
            spec(kDefaultSpec)
        {}

        static void PrintUsage();

        int Parse(int argc, char* argv[]);

        bool forced;
        std::string outDir;
        bool recursive;
        std::string spec;

    private:
        static const char* kDefaultSpec;
    };
}

#endif  // X2BOOST_XPILER_OPTIONS_HPP_
