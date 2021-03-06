// Copyright (c) 2014-2017 Jae-jun Kang
// See the file LICENSE for details.

#ifndef X2BOOST_BINDER_HPP_
#define X2BOOST_BINDER_HPP_

#ifndef X2BOOST_PRE_HPP_
#include "x2boost/pre.hpp"
#endif

#include <boost/thread/shared_mutex.hpp>
#include <boost/unordered_map.hpp>

#include "x2boost/event.hpp"
#include "x2boost/fingerprint.hpp"
#include "x2boost/handler.hpp"

namespace x2boost
{
    class binder
    {
    public:
        binder() {}

        void bind(event_ptr e, handler_ptr handler);
        void unbind(event_ptr e, handler_ptr handler);

        std::size_t build_handler_chain(event_ptr e, handler_chain_type& handler_chain) const;

    private:
        class filter
        {
        public:
            typedef std::vector<slot> list_type;

            filter() {}

            void add(boost::int32_t type_id, const fingerprint& fp);
            void remove(boost::int32_t type_id, const fingerprint& fp);

            const list_type* get(boost::int32_t type_id) const
            {
                map_type::const_iterator it = store_.find(type_id);
                return (it != store_.end() ? it->second : 0);
            }

        private:
            typedef boost::unordered_map<boost::int32_t, list_type*> map_type;

            map_type store_;
        };

        class handler_set
        {
        public:
            typedef std::vector<handler_ptr> list_type;

            handler_set() {}
            
            bool add(handler_ptr handler);
            bool remove(handler_ptr handler);

            bool empty() { return store_.empty(); }

            const list_type& get() const { return store_; }

        private:
            list_type store_;
        };

        struct h
        {
            std::size_t operator()(const event_ptr& x) const
            {
                return x->_hash_code();
            }
        };

        struct eq
        {
            bool operator()(const event_ptr& x, const event_ptr& y) const
            {
                return x->_equals(*y);
            }
        };

        typedef boost::unordered_map<event_ptr, handler_set*, h, eq> map_type;

        filter filter_;
        map_type map_;

        mutable boost::shared_mutex mutex_;
    };
}

#endif  // X2BOOST_BINDER_HPP_
