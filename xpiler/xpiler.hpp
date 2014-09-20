// Copyright (c) 2014 Jae-jun Kang
// See the file COPYING for license details.

#ifndef X2BOOST_XPILER_XPILER_HPP_
#define X2BOOST_XPILER_XPILER_HPP_

#ifndef X2BOOST_XPILER_PRE_HPP_
#include "pre.hpp"
#endif

#include <map>

#include "formatter.hpp"
#include "handler.hpp"

namespace xpiler
{
    struct Options;

    class xpiler
    {
    public:
        typedef std::map<std::string, HandlerPtr> HandlerMapType;
        typedef std::map<std::string, FormatterPtr> FormatterMapType;

        xpiler();

        static const FormatterMapType& GetFormatters()
        {
            return formatters_;
        }

        void Process(const std::string& path);

        static Options options;

        bool error;

    private:
        void ProcessDir(const std::string& path);
        void ProcessFile(const std::string& path);

        static HandlerMapType handlers_;
        static FormatterMapType formatters_;

        struct StaticInitializer
        {
            StaticInitializer();
        };
        static StaticInitializer static_init_;

        FormatterPtr formatter_;
        std::vector<std::string> sub_dirs_;
    };
}

#endif  // X2BOOST_XPILER_XPILER_HPP_
