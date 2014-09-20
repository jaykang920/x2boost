// Copyright (c) 2014 Jae-jun Kang
// See the file COPYING for license details.

#include "x2boost/hub.hpp"

#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/thread/locks.hpp>

#include "x2boost/event.hpp"
#include "x2boost/flow.hpp"

using namespace x2;

hub hub::instance_;

hub& hub::attach(flow_ptr flow)
{
    boost::upgrade_lock<boost::shared_mutex> lock(mutex_);
    if (std::find(flows_.begin(), flows_.end(), flow) == flows_.end())
    {
        boost::upgrade_to_unique_lock<boost::shared_mutex> unique_lock(lock);
        flows_.push_back(flow);
    }
    return *this;
}

hub& hub::detach(flow_ptr flow)
{
    boost::upgrade_lock<boost::shared_mutex> lock(mutex_);
    std::vector<flow_ptr>::iterator it = std::find(flows_.begin(), flows_.end(), flow);
    if (it != flows_.end())
    {
        boost::upgrade_to_unique_lock<boost::shared_mutex> unique_lock(lock);
        flows_.erase(it);
        unsubscribe_internal(flow);
    }
    return *this;
}

void hub::detach_all()
{
    boost::unique_lock<boost::shared_mutex> lock(mutex_);
    subscriptions_.clear();
    flows_.clear();
}

void hub::feed(event_ptr e) const
{
    boost::shared_lock<boost::shared_mutex> lock(mutex_);
    const std::vector<flow_ptr>* flows;
    const char* channel = e->_channel();
    if (channel == 0 || *channel == '\0')
    {
        flows = &flows_;
    }
    else
    {
        map_type::const_iterator it = subscriptions_.find(channel);
        if (it == subscriptions_.end())
        {
            return;
        }
        flows = &(it->second);
    }
    std::for_each(flows->begin(), flows->end(), boost::bind(&flow::feed, _1, e));
}

void hub::start_attached_flows() const
{
    boost::shared_lock<boost::shared_mutex> lock(mutex_);
    std::for_each(flows_.begin(), flows_.end(), boost::mem_fn(&flow::startup));
}

void hub::stop_attached_flows() const
{
    boost::shared_lock<boost::shared_mutex> lock(mutex_);
    std::for_each(flows_.begin(), flows_.end(), boost::mem_fn(&flow::shutdown));
}

void hub::subscribe(flow_ptr flow, const char* channel)
{
    boost::unique_lock<boost::shared_mutex> lock(mutex_);
    subscribe_internal(flow, channel);
}

void hub::unsubscribe(flow_ptr flow, const char* channel)
{
    boost::unique_lock<boost::shared_mutex> lock(mutex_);
    unsubscribe_internal(flow, channel);
}

void hub::subscribe_internal(flow_ptr flow, const char* channel)
{
    if (channel == 0 || *channel == '\0')
    {
        // invalid channel name
        return;
    }
    std::vector<flow_ptr>* flows;
    map_type::iterator it = subscriptions_.find(channel);
    if (it != subscriptions_.end())
    {
        flows = &(it->second);
    }
    else
    {
        std::vector<flow_ptr> v;
        std::pair<map_type::iterator, bool> result =
            subscriptions_.insert(std::make_pair(channel, v));
        flows = &(result.first->second);
    }
    if (std::find(flows->begin(), flows->end(), flow) == flows->end())
    {
        flows->push_back(flow);
    }
}

void hub::unsubscribe_internal(flow_ptr flow)
{
    std::vector<const char*> keys_to_remove;
    BOOST_FOREACH(map_type::value_type& pair, subscriptions_)
    {
        std::vector<flow_ptr>& v = pair.second;
        v.erase(std::remove(v.begin(), v.end(), flow), v.end());
        if (v.empty())
        {
            keys_to_remove.push_back(pair.first);
        }
    }
    BOOST_FOREACH(const char* key, keys_to_remove)
    {
        subscriptions_.erase(key);
    }
}

void hub::unsubscribe_internal(flow_ptr flow, const char* channel)
{
    if (channel == 0 || *channel == '\0')
    {
        // invalid channel name
        return;
    }
    map_type::iterator it = subscriptions_.find(channel);
    if (it == subscriptions_.end())
    {
        return;
    }
    std::vector<flow_ptr>& v = it->second;
    v.erase(std::remove(v.begin(), v.end(), flow), v.end());
    if (v.empty())
    {
        subscriptions_.erase(it);
    }
}

// EOF hub.cpp
