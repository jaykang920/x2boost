// Copyright (c) 2014-2015 Jae-jun Kang
// See the file LICENSE for details.

#ifndef X2BOOST_EVENT_FACTORY_HPP_
#define X2BOOST_EVENT_FACTORY_HPP_

#ifndef X2BOOST_PRE_HPP_
#include "x2boost/pre.hpp"
#endif

#include <boost/unordered_map.hpp>

#include "x2boost/event.hpp"

namespace x2
{
    class deserializer;

    class X2BOOST_API event_factory
    {
    public:
        typedef event_ptr(*func_type)();

        static event_ptr create(deserializer& deserializer);

        static event_ptr create(boost::int32_t type_id)
        {
            map_type::iterator it = instance().registry().find(type_id);
            if (it == instance().registry().end())
            {
                log::error()
                    << "event_factory::create : unknown event type id "
                    << type_id << std::endl;
                return event_ptr();
            }
            return (*it->second)();
        }

        static event_factory& instance();

        template<class T>  // T : event
        static void enroll(boost::int32_t type_id)
        {
            enroll(type_id, T::_new);
        }

        static void enroll(boost::int32_t type_id, func_type func)
        {
            map_type::iterator it = instance().registry().find(type_id);
            if (it != instance().registry().end())
            {
                if (it->second != func)
                {
                    log::error()
                        << "event type id " << type_id << " conflicted"
                        << std::endl;
                }
                return;
            }
            instance().registry().insert(map_type::value_type(type_id, func));
        }

    private:
        typedef boost::unordered_map<boost::int32_t, func_type> map_type;

        event_factory() {}

        map_type& registry() { return registry_; }

        map_type registry_;
    };
}

#endif  // X2BOOST_EVENT_FACTORY_HPP_
