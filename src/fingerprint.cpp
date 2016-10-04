// Copyright (c) 2014-2016 Jae-jun Kang
// See the file LICENSE for details.

#include "x2boost/fingerprint.hpp"

#include <boost/functional/hash.hpp>

#include "x2boost/deserializer.hpp"
#include "x2boost/serializer.hpp"

using namespace x2;

fingerprint::fingerprint(std::size_t length)
  : length_((int)length),
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
        int n = other.num_additional_blocks();
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
        for (int i = 0, count = num_additional_blocks(); i < count; ++i)
        {
            if (blocks_[i] != other.blocks_[i])
            {
                return false;
            }
        }
    }
    return true;
}

bool fingerprint::equivalent(const fingerprint& other) const
{
    if (this == &other)
    {
        return true;
    }
    if (length_ > other.length_)
    {
        return false;
    }
    if ((block_ & other.block_) != block_)
    {
        return false;
    }
    if (blocks_)
    {
        for (int i = 0, count = num_additional_blocks(); i < count; ++i)
        {
            std::size_t mine = blocks_[i];
            if ((mine & other.blocks_[i]) != mine)
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

void fingerprint::deserialize(deserializer& deserializer)
{
    boost::int32_t length;
    deserializer.read_variable_nonnegative(length);
    int length_in_bytes = ((length - 1) >> 3) + 1;
    int length_in_blocks = ((length_in_bytes - 1) >> 2) + 1;
    int effective_bytes = this->length_in_bytes();

    int count = 0;
    block_ = 0;
    for (int i = 0; (i < 4) && (count < length_in_bytes); ++i, ++count)
    {
        byte_t b;
        deserializer.read(b);
        if (count < effective_bytes)
        {
            block_ |= ((std::size_t)b << (i << 3));
        }
    }
    for (int i = 0; i < length_in_blocks; ++i)
    {
        std::size_t word = 0;
        for (int j = 0; (j < 4) && (count < length_in_bytes); ++j, ++count)
        {
            byte_t b;
            deserializer.read(b);
            if (count < effective_bytes)
            {
                word |= ((std::size_t)b << (j << 3));
            }
        }
        if (blocks_ != NULL && i < num_additional_blocks())
        {
            blocks_[i] = word;
        }
    }
}

int fingerprint::get_encoded_length() const
{
    return serializer::get_encoded_length_variable_nonnegative(length_)
        + length_in_bytes();
}

void fingerprint::serialize(serializer& serializer) const
{
    serializer.write_variable_nonnegative(length_);
    int length_in_bytes = this->length_in_bytes();
    int num_additional_blocks = this->num_additional_blocks();

    int count = 0;
    for (int i = 0; (i < 4) && (count < length_in_bytes); ++i, ++count)
    {
        serializer.write((byte_t)(block_ >> (i << 3)));
    }
    if (blocks_ == NULL)
    {
        return;
    }
    for (int i = 0; i < num_additional_blocks; ++i)
    {
        for (int j = 0; (j < 4) && (count < length_in_bytes); ++j, ++count)
        {
            serializer.write((byte_t)(blocks_[i] >> (j << 3)));
        }
    }
}


// EOF fingerprint.cpp
