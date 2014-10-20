// Copyright (c) 2014 Jae-jun Kang
// See the file COPYING for license details.

#include "x2boost/cell.hpp"

#include <sstream>

#include <boost/thread/once.hpp>

using namespace x2;

namespace
{
    cell::tag cell_tag;
    boost::once_flag cell_once = BOOST_ONCE_INIT;

    void cell_init()
    {
        cell_tag.set(NULL, 0);
    }
}

bool cell::_equals(const cell& other) const
{
    if (this == &other)
    {
        return true;
    }
    if (_type_tag() != other._type_tag())
    {
        return false;
    }
    if (fingerprint_ != other.fingerprint_)
    {
        return false;
    }
    return true;
}

bool cell::_equivalent(const cell& other) const
{
    if (this == &other)
    {
        return true;
    }
    if (!other._is_kind_of(*this))
    {
        return false;
    }
    if (!fingerprint_.equivalent(other.fingerprint_))
    {
        return false;
    }
    return true;
}

std::size_t cell::_hash_code() const
{
    return _hash_code(fingerprint_);
}

std::size_t cell::_hash_code(const fingerprint& fingerprint) const
{
    return 17;
}

std::string cell::_string() const
{
    std::ostringstream oss;
    _describe(oss);
    return oss.str();
}

const cell::tag* cell::_tag()
{
    boost::call_once(&cell_init, cell_once);
    return &cell_tag;
}

bool cell::_is_kind_of(const cell& other) const
{
    const tag* t = _type_tag();
    const tag* t_ = other._type_tag();
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

const cell::tag* cell::_type_tag() const
{
    return _tag();
}

void cell::_describe(std::ostream& /*stream*/) const
{
    return;
}

// EOF cell.cpp
