// Copyright (c) 2014-2015 Jae-jun Kang
// See the file LICENSE for details.

#include "x2boost/buffer.hpp"

#include "x2boost/util/buffer_pool.hpp"

using namespace x2;

buffer::buffer(int block_size_exponent)
{
    BOOST_VERIFY(buffer_pool::min_size_exponent <= block_size_exponent);
    BOOST_VERIFY(block_size_exponent <= buffer_pool::max_size_exponent);

    block_size_exponent_ = block_size_exponent;
    remainder_mask_ = ~(~0 << block_size_exponent);

    blocks_.push_back(buffer_pool::acquire(block_size_exponent));

    block_index_ = 0;
    block_ = blocks_[block_index_];

    pos_ = back_ = front_ = 0;
    marker_ = -1;
}

void buffer::cleanup()
{
    if (blocks_.empty())
    {
        return;
    }
    for (std::size_t i = 0, count = blocks_.size(); i < count; ++i)
    {
        buffer_pool::release(block_size_exponent_, blocks_[i]);
    }
    blocks_.clear();
    block_index_ = -1;
    block_ = 0;
}

void buffer::ensure_capacity_to_write(int n)
{
    int required = pos_ + n;
    while (required >= capacity())
    {
        blocks_.push_back(buffer_pool::acquire(block_size_exponent_));
    }
    if (required > back_)
    {
        back_ = required;
    }
}

void buffer::pos(int value)
{
    int adjusted = value + front_;
    BOOST_ASSERT(front_ <= adjusted && adjusted <= back_);
    pos_ = adjusted;

    int block_index = pos_ >> block_size_exponent_;
    if ((block_index != 0) && ((pos_ & remainder_mask_) == 0))
    {
        --block_index;
    }
    if (block_index != block_index_)
    {
        block_index_ = block_index;
        block_ = blocks_[block_index_];
    }
}

// EOF buffer.cpp
