// Copyright (c) 2014 Jae-jun Kang
// See the file COPYING for license details.

#include "x2boost/cell.hpp"

#include <sstream>

#include <boost/thread/once.hpp>

using namespace x2;

namespace
{
    cell::tag t;
    boost::once_flag once = BOOST_ONCE_INIT;

    void init()
    {
        t.set(NULL, 0);
    }
}

bool cell::equals(const cell& other) const
{
    if (this == &other)
    {
        return true;
    }
    if (type_tag() != other.type_tag())
    {
        return false;
    }
    if (fingerprint_ != other.fingerprint_)
    {
        return false;
    }
    return true;
}

bool cell::equivalent(const cell& other) const
{
    if (this == &other)
    {
        return true;
    }
    if (!other.is_kind_of(*this))
    {
        return false;
    }
    if (!fingerprint_.equivalent(other.fingerprint_))
    {
        return false;
    }
    return true;
}

std::size_t cell::hash_code() const
{
    return hash_code(fingerprint_);
}

std::size_t cell::hash_code(const fingerprint& fingerprint) const
{
    return 17;
}

std::string cell::string() const
{
    std::ostringstream oss;
    describe(oss);
    return oss.str();
}

const cell::tag* cell::_tag()
{
    boost::call_once(&init, once);
    return &t;
}

bool cell::is_kind_of(const cell& other) const
{
    const tag* t = type_tag();
    const tag* t_ = other.type_tag();
    while (t)
    {
        if (t == t_)
        {
            return true;
        }
        t = t->base();
    }
    return false;
}

const cell::tag* cell::type_tag() const
{
    return _tag();
}

void cell::describe(std::ostream& /*stream*/) const
{
    return;
}

// EOF cell.cpp
