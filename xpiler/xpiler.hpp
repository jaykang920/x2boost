// Copyright (c) 2014-2015 Jae-jun Kang
// See the file LICENSE for details.

#ifndef X2BOOST_XPILER_XPILER_HPP_
#define X2BOOST_XPILER_XPILER_HPP_

#ifndef X2BOOST_XPILER_PRE_HPP_
#include "pre.hpp"
#endif

#include <map>

#include <boost/filesystem.hpp>

#include "formatter.hpp"
#include "handler.hpp"
#include "options.hpp"

namespace xpiler
{
    class xpiler
    {
    public:
        typedef std::map<std::string, handler*> handler_map_type;
        typedef std::map<std::string, formatter*> formatter_map_type;

        xpiler();

        void process(const std::string& path);

        static const formatter_map_type& formatters() { return formatters_; }

        static options opts;

        bool error;

    private:
        void process_dir(const boost::filesystem::path& path);
        void process_file(const boost::filesystem::path& path);

        static handler_map_type handlers_;
        static formatter_map_type formatters_;

        struct static_initializer
        {
            static_initializer();
            ~static_initializer();
        };
        static static_initializer static_init_;

        formatter* formatter_;
        std::vector<std::string> sub_dirs_;
    };
}

#endif  // X2BOOST_XPILER_XPILER_HPP_
