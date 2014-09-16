// Copyright (c) 2014 Jae-jun Kang
// See the file COPYING for license details.

#ifndef X2BOOST_XPILER_XPILER_HPP_
#define X2BOOST_XPILER_XPILER_HPP_

#ifndef X2BOOST_XPILER_PRE_HPP_
#include "pre.hpp"
#endif

namespace x2boost
{
    struct Options;

    class Xpiler
    {
    public:
        static Options options;

        bool error;

        void Process(std::string& path) {}

    private:
    };
}

#endif  // X2BOOST_XPILER_XPILER_HPP_
