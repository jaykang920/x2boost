// Copyright (c) 2014 Jae-jun Kang
// See the file COPYING for license details.

#include "x2boost/event.hpp"

#include <boost/functional/hash.hpp>
#include <boost/thread/once.hpp>

using namespace x2;

namespace
{
    event::tag event_tag;
    boost::once_flag event_once = BOOST_ONCE_INIT;

    void event_init()
    {
        event_tag.set(NULL, 0, 0);
    }
}

bool event::_equals(const cell& other) const
{
    if (!cell::_equals(other))
    {
        return false;
    }
    // channel?
    return true;
}

std::size_t event::_hash_code() const
{
    return _hash_code(fingerprint_, _type_id());
}

std::size_t event::_hash_code(const fingerprint& fp) const
{
    std::size_t value = cell::_hash_code(fp);
    //boost::hash_combine(value, x)
    return value;
}

std::size_t event::_hash_code(const fingerprint& fp, boost::int32_t type_id) const
{
    std::size_t value = 0;
    boost::hash_combine(value, type_id);
    boost::hash_combine(value, _hash_code(fp));
    return value;
}

const event::tag* event::_tag()
{
    boost::call_once(&event_init, event_once);
    return &event_tag;
}

const cell::tag* event::_type_tag() const
{
    return _tag();
}

void event::_describe(std::ostream& /*stream*/) const
{
    return;
}

// EOF event.cpp
