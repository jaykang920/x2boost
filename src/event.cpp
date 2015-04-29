// Copyright (c) 2014-2015 Jae-jun Kang
// See the file LICENSE for details.

#include "x2boost/event.hpp"

#include <boost/functional/hash.hpp>
#include <boost/thread/once.hpp>

#include "x2boost/deserializer.hpp"
#include "x2boost/event_factory.hpp"
#include "x2boost/serializer.hpp"

using namespace x2;

namespace
{
    event::tag event_tag;
    boost::once_flag event_once = BOOST_ONCE_INIT;

    void event_init()
    {
        event_tag.set(NULL, 1, 0);
    }

    struct static_event_initializer
    {
        static_event_initializer()
        {
            event_factory::enroll(0, event::_new);
        }
    };
    static_event_initializer static_event_init;
}

bool event::_equals(const cell& other) const
{
    if (!cell::_equals(other))
    {
        return false;
    }
    const event& o = static_cast<const event&>(other);
    if (_handle_ != o._handle_)
    {
        return false;
    }
    return true;
}

bool event::_equivalent(const cell& other) const
{
    if (!cell::_equivalent(other))
    {
        return false;
    }
    const event& o = static_cast<const event&>(other);
    if (fingerprint_[_tag()->offset() + 0])
    {
        if (_handle_ != o._handle_)
        {
            return false;
        }
    }
    return true;
}

std::size_t event::_hash_code() const
{
    return _hash_code(fingerprint_, _type_id());
}

std::size_t event::_hash_code(const fingerprint& fp) const
{
    std::size_t value = cell::_hash_code(fp);
    if (fingerprint_[_tag()->offset() + 0])
    {
        boost::hash_combine(value, _handle_);
    }
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
    boost::call_once(event_init, event_once);
    return &event_tag;
}

const cell::tag* event::_type_tag() const
{
    return _tag();
}

void event::_describe(std::ostringstream& /*oss*/) const
{
    return;
}

void event::_deserialize(deserializer& deserializer)
{
    cell::_deserialize(deserializer);
}

int event::_get_encoded_length() const
{
    int length = serializer::get_encoded_length((boost::int32_t)_type_id());
    length += cell::_get_encoded_length();
    return length;
}

void event::_serialize(serializer& serializer) const
{
    serializer.write((boost::int32_t)_type_id());
    cell::_serialize(serializer);
}

// EOF event.cpp
