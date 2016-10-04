// Copyright (c) 2014-2016 Jae-jun Kang
// See the file LICENSE for details.

#ifndef X2BOOST_XPILER_OPTIONS_HPP_
#define X2BOOST_XPILER_OPTIONS_HPP_

#ifndef X2BOOST_XPILER_PRE_HPP_
#include "pre.hpp"
#endif

namespace xpiler
{
    struct options
    {
        options()
          : forced(false),
            recursive(false),
            spec(default_spec)
        {}

        bool parse(int argc, char* argv[]);

        bool forced;
        std::vector<std::string> input;
        std::string out_dir;
        bool recursive;
        std::string spec;

    private:
        static const char* default_spec;
    };
}

#endif  // X2BOOST_XPILER_OPTIONS_HPP_
