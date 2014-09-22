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
    struct options;

    class xpiler
    {
    public:
        typedef std::map<std::string, handler_ptr> handler_map_type;
        typedef std::map<std::string, formatter_ptr> formatter_map_type;

        xpiler();

        void process(const std::string& path);

        static const formatter_map_type& formatters() { return formatters_; }

        static options options;

        bool error;

    private:
        void process_dir(const std::string& path);
        void process_file(const std::string& path);

        static handler_map_type handlers_;
        static formatter_map_type formatters_;

        struct static_initializer
        {
            static_initializer();
        };
        static static_initializer static_init_;

        formatter_ptr formatter_;
        std::vector<std::string> sub_dirs_;
    };
}

#endif  // X2BOOST_XPILER_XPILER_HPP_
