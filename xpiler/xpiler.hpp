// Copyright (c) 2014 Jae-jun Kang
// See the file COPYING for license details.

#ifndef X2BOOST_XPILER_XPILER_HPP_
#define X2BOOST_XPILER_XPILER_HPP_

#ifndef X2BOOST_XPILER_PRE_HPP_
#include "pre.hpp"
#endif

#include <map>

namespace x2boost
{
    struct InputHandler;
    struct Options;
    struct OutputFormatter;

    class Xpiler
    {
    public:
        Xpiler();

        void Process(const std::string& path);

        static Options options;

        bool error;

    private:
        typedef std::map<std::string, InputHandler*> HandlerMapType;
        typedef std::map<std::string, OutputFormatter*> FormatterMapType;

        void ProcessDir(const std::string& path);
        void ProcessFile(const std::string& path);

        static HandlerMapType handlers_;
        static FormatterMapType formatters_;

        struct StaticInitializer
        {
            StaticInitializer();
        };
        static StaticInitializer static_init_;

        OutputFormatter* formatter_;
        std::vector<std::string> sub_dirs_;
    };
}

#endif  // X2BOOST_XPILER_XPILER_HPP_
