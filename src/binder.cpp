// Copyright (c) 2014 Jae-jun Kang
// See the file COPYING for license details.

#include "x2boost/binder.hpp"

using namespace x2;

void binder::filter::add(boost::int32_t type_id, const fingerprint& fp)
{
    map_type::iterator it = map_.find(type_id);
    if (it != map_.end())
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
        map_.insert(map_type::value_type(type_id, slots));
    }
}

void binder::filter::remove(boost::int32_t type_id, const fingerprint& fp)
{
    map_type::iterator it = map_.find(type_id);
    if (it == map_.end())
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
        map_.erase(it);
    }
}

// EOF binder.cpp
