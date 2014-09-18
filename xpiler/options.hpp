// Copyright (c) 2014 Jae-jun Kang
// See the file COPYING for license details.

#ifndef X2BOOST_XPILER_OPTIONS_HPP_
#define X2BOOST_XPILER_OPTIONS_HPP_

#ifndef X2BOOST_XPILER_PRE_HPP_
#include "pre.hpp"
#endif

namespace xpiler
{
    struct Options
    {
        Options()
          : forced(false),
            recursive(false),
            spec(kDefaultSpec)
        {}

        bool Parse(int argc, char* argv[]);

        bool forced;
        std::vector<std::string> input;
        std::string out_dir;
        bool recursive;
        std::string spec;

    private:
        static const char* kDefaultSpec;
    };
}

#endif  // X2BOOST_XPILER_OPTIONS_HPP_
