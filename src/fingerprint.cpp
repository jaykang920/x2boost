// Copyright (c) 2014 Jae-jun Kang
// See the file COPYING for license details.

#include "x2boost/fingerprint.hpp"

#include <boost/functional/hash.hpp>

using namespace x2;

fingerprint::fingerprint(std::size_t length)
  : length_(length),
    block_(0),
    blocks_(NULL)
{
    if (length > bits_per_block)
    {
        blocks_ = new std::size_t[num_additional_blocks()];
    }
}

fingerprint::fingerprint(const fingerprint& other)
  : length_(other.length_),
    block_(other.block_),
    blocks_(NULL)
{
    if (other.blocks_)
    {
        std::size_t n = other.num_additional_blocks();
        blocks_ = new std::size_t[n];
        ::memcpy(blocks_, other.blocks_, sizeof(std::size_t) * num_additional_blocks());
    }
}

fingerprint::~fingerprint()
{
    if (blocks_)
    {
        delete[] blocks_;
        blocks_ = NULL;
    }
}

void fingerprint::clear()
{
    block_ = 0;
    if (blocks_)
    {
        ::memset(blocks_, 0, sizeof(std::size_t) * num_additional_blocks());
    }
}

bool fingerprint::operator<(const fingerprint& other) const
{
    if (this == &other)
    {
        return false;
    }
    if (length_ < other.length_)
    {
        return true;
    }
    else if (length_ > other.length_)
    {
        return false;
    }
    if (blocks_)
    {
        for (int i = (int)(num_additional_blocks() - 1); i >= 0; --i)
        {
            std::size_t mine = blocks_[i];
            std::size_t others = other.blocks_[i];
            if (mine < others)
            {
                return true;
            }
            else if (mine > others)
            {
                return false;
            }
        }
    }
    if (block_ < other.block_)
    {
        return true;
    }
    return false;
}

bool fingerprint::operator==(const fingerprint& other) const
{
    if (this == &other)
    {
        return true;
    }
    if (length_ != other.length_)
    {
        return false;
    }
    if (block_ != other.block_)
    {
        return false;
    }
    if (blocks_)
    {
        for (std::size_t i = 0, count = num_additional_blocks(); i < count; ++i)
        {
            if (blocks_[i] != other.blocks_[i])
            {
                return false;
            }
        }
    }
    return true;
}

std::size_t fingerprint::hash_code() const
{
    std::size_t value = 17;
    boost::hash_combine(value, length_);
    boost::hash_combine(value, block_);
    if (blocks_)
    {
        for (std::size_t i = 0, count = num_additional_blocks(); i < count; ++i)
        {
            boost::hash_combine(value, blocks_[i]);
        }
    }
    return value;
}

void fingerprint::swap(fingerprint& other)
{
    std::swap(length_, other.length_);
    std::swap(block_, other.block_);
    std::swap(blocks_, other.blocks_);
}

// EOF fingerprint.cpp
