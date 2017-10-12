// Copyright (c) 2014-2017 Jae-jun Kang
// See the file LICENSE for details.

#include "x2boost/buffer.hpp"

#include <algorithm>
#include <cstring>

#include <boost/asio/buffer.hpp>

#include "x2boost/util/buffer_pool.hpp"

using namespace x2boost;

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

void buffer::copy_from(int pos, byte_t* buffer, int offset, int count)
{
    int block_index = pos >> block_size_exponent_;
    int dst_offset = pos & remainder_mask_;
    int bytes_to_copy, bytes_copied = 0;
    while (bytes_copied < count)
    {
        bytes_to_copy = std::min(block_size() - dst_offset, count - bytes_copied);
        ::memcpy(blocks_[block_index++] + dst_offset,
            buffer + offset + bytes_copied, bytes_to_copy);
        dst_offset = 0;
        bytes_copied += bytes_to_copy;
    }
}

void buffer::copy_to(int pos, byte_t* buffer, int offset, int count) const
{
    int block_index = pos >> block_size_exponent_;
    int src_offset = pos & remainder_mask_;
    int bytes_to_copy, bytes_copied = 0;
    while (bytes_copied < count)
    {
        bytes_to_copy = std::min(block_size() - src_offset, count - bytes_copied);
        ::memcpy(buffer + offset + bytes_copied,
            blocks_[block_index++] + src_offset, bytes_to_copy);
        src_offset = 0;
        bytes_copied += bytes_to_copy;
    }
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

void buffer::list_const_buffers(std::vector<boost::asio::const_buffer>& result, int begin, int end) const
{
    int begin_index = begin >> block_size_exponent_;
    int begin_offset = begin & remainder_mask_;
    int end_index = end >> block_size_exponent_;
    int end_offset = end & remainder_mask_;
    if (begin_index == end_index)
    {
        result.push_back(boost::asio::buffer(blocks_[begin_index] + begin_offset, end_offset - begin_offset));
        return;
    }
    result.push_back(boost::asio::buffer(blocks_[begin_index] + begin_offset, block_size() - begin_offset));
    for (int i = begin_index + 1; i < end_index; ++i)
    {
        result.push_back(boost::asio::buffer(blocks_[i], block_size()));
    }
    if (end_offset != 0)
    {
        result.push_back(boost::asio::buffer(blocks_[end_index], end_offset));
    }
}

void buffer::list_mutable_buffers(std::vector<boost::asio::mutable_buffer>& result)
{
    if ((capacity() - back_) < block_size())
    {
        blocks_.push_back(buffer_pool::acquire(block_size_exponent_));
    }
    int back_index = back_ >> block_size_exponent_;
    int back_offset = back_ & remainder_mask_;
    result.push_back(boost::asio::buffer(blocks_[back_index] + back_offset, block_size() - back_offset));
    for (int i = back_index + 1; i < (int)blocks_.size(); ++i)
    {
        result.push_back(boost::asio::buffer(blocks_[i], block_size()));
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

void buffer::read(byte_t* buffer, int offset, int count)
{
    check_length_to_read(count);
    copy_to(pos_, buffer, offset, count);
    pos(pos() + count);
}

void buffer::shrink(int length)
{
    if ((front_ + length) > back_)
    {
        // exception
        return;
    }
    front_ += length;
    if (pos_ < front_)
    {
        pos(0);
    }
}

void buffer::stretch(int length)
{
    if ((back_ + length) > capacity())
    {
        // exception
        return;
    }
    back_ += length;
}

byte_t* buffer::to_array() const
{
    byte_t* array = new byte_t[length()];
    copy_to(front_, array, 0, length());
    return array;
}

void buffer::trim()
{
    int index, count;
    if (marker_ >= 0)
    {
        if (pos_ < marker_)
        {
            pos(marker_ - front_);
        }
        marker_ = -1;
    }
    if (pos_ == back_)
    {
        index = 1;
        count = (int)blocks_.size() - 1;
        front_ = back_ = 0;
    }
    else
    {
        index = 0;
        count = pos_ >> block_size_exponent_;
        if (count >= (int)blocks_.size())
        {
            count = (int)blocks_.size() - 1;
        }
        back_ -= block_size() * count;
        front_ = pos_ & remainder_mask_;
    }
    if (count > 0)
    {
        int limit = index + count;
        for (int i = index; i < limit; ++i)
        {
            buffer_pool::release(block_size_exponent_, blocks_[i]);
        }
        blocks_.erase(blocks_.begin() + index, blocks_.begin() + limit);
    }
    pos(0);
}

void buffer::write(byte_t* buffer, int offset, int count)
{
    ensure_capacity_to_write(count);
    copy_from(pos_, buffer, offset, count);
    pos(pos() + count);
}

// EOF buffer.cpp
