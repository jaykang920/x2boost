// Copyright (c) 2014 Jae-jun Kang
// See the file COPYING for license details.

#include "x2boost/binder.hpp"

#include <boost/thread/locks.hpp>

#include "x2boost/event.hpp"

namespace
{
    template<typename T>
    struct xx
    {
        boost::shared_ptr<T> operator()(const boost::shared_ptr<T>& x)
        {
            return x;
        }
    };
}

using namespace x2;

void binder::bind(event_ptr e, handler_ptr handler)
{
    boost::unique_lock<boost::shared_mutex> lock(mutex_);

    handler_set* handlers;
    map_type::iterator it = map_.find(e);
    if (it != map_.end())
    {
        handlers = it->second;
    }
    else
    {
        handlers = new handler_set;
        map_.insert(map_type::value_type(e, handlers));
    }

    if (handlers->add(handler))
    {
        filter_.add(e->_type_id(), e->_fingerprint());

        // handle event sink
    }
}

void binder::unbind(event_ptr e, handler_ptr handler)
{
    boost::unique_lock<boost::shared_mutex> lock(mutex_);

    map_type::iterator it = map_.find(e);
    if (it == map_.end())
    {
        return;
    }
    handler_set* handlers = it->second;
    if (!handlers->remove(handler))
    {
        return;
    }
    if (handlers->empty())
    {
        delete handlers;
        map_.erase(e);
    }
    filter_.remove(e->_type_id(), e->_fingerprint());

    // handle event sink
}

std::size_t binder::build_handler_chain(event_ptr e, handler_chain_type& handler_chain) const
{
    boost::shared_lock<boost::shared_mutex> lock(mutex_);

    const event::tag* t = static_cast<const event::tag*>(e->_type_tag());
    const fingerprint& fp = e->_fingerprint();
    while (t)
    {
        int type_id = t->type_id();
        const filter::list_type* slots = filter_.get(type_id);
        if (slots)
        {
            for (std::size_t i = 0, count = slots->size(); i < count; ++i)
            {
                const slot& s = slots->at(i);
                if (s.equivalent(fp))
                {
                    // equiv
                    map_type::const_iterator it = map_.find(e);
                    if (it != map_.end())
                    {
                        const handler_set::list_type& handlers = it->second->get();
                        std::transform(handlers.begin(), handlers.end(),
                            std::back_inserter(handler_chain), xx<handler>());
                    }
                }
            }
        }
        t = static_cast<const event::tag*>(t->base());
    }
    return handler_chain.size();
}

void binder::filter::add(boost::int32_t type_id, const fingerprint& fp)
{
    map_type::iterator it = store_.find(type_id);
    if (it != store_.end())
    {
        slot s(fp);
        list_type* slots = it->second;
        list_type::iterator i = std::lower_bound(slots->begin(), slots->end(), s);
        if (i != slots->end())
        {
            if (*i == s)
            {
                i->add_ref();
            }
            else
            {
                slots->insert(i, s);
            }
        }
        else
        {
            slots->push_back(s);
        }
    }
    else
    {
        list_type* slots = new list_type;
        slots->push_back(slot(fp));
        store_.insert(map_type::value_type(type_id, slots));
    }
}

void binder::filter::remove(boost::int32_t type_id, const fingerprint& fp)
{
    map_type::iterator it = store_.find(type_id);
    if (it == store_.end())
    {
        return;
    }
    slot s(fp);
    list_type* slots = it->second;
    list_type::iterator i = std::lower_bound(slots->begin(), slots->end(), s);
    if (i == slots->end() || *i != s)
    {
        return;
    }
    if (i->remove_ref() == 0)
    {
        slots->erase(i);
    }
    if (slots->empty())
    {
        delete slots;
        store_.erase(it);
    }
}

bool binder::handler_set::add(handler_ptr handler)
{
    list_type::iterator it = std::find(store_.begin(), store_.end(), handler);
    if (it != store_.end())
    {
        return false;
    }
    store_.push_back(handler);
    return true;
}

bool binder::handler_set::remove(handler_ptr handler)
{
    list_type::iterator it = std::find(store_.begin(), store_.end(), handler);
    if (it == store_.end())
    {
        return false;
    }
    store_.erase(it);
    return true;
}


// EOF binder.cpp
