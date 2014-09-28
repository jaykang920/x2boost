// Copyright (c) 2014 Jae-jun Kang
// See the file COPYING for license details.

#ifndef X2BOOST_BINDER_HPP_
#define X2BOOST_BINDER_HPP_

#ifndef X2BOOST_PRE_HPP_
#include "x2boost/pre.hpp"
#endif

#include <boost/unordered_map.hpp>

#include "x2boost/fingerprint.hpp"

namespace x2
{
    class binder
    {
    public:

    private:
        class filter
        {
        public:
            void add(boost::int32_t type_id, const fingerprint& fp);
            void remove(boost::int32_t type_id, const fingerprint& fp);

        private:
            typedef std::vector<slot> list_type;
            typedef boost::unordered_map<boost::int32_t, list_type*> map_type;

            map_type map_;
        };

        class handler_set
        {
        public:
        private:
            //typedef std::vector<> list_type;
        };

        typedef boost::unordered_map<event_ptr, handler_set> map_type;

        filter filter_;
        map_type map_;
    };
}

#endif  // X2BOOST_BINDER_HPP_
