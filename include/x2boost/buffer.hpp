// Copyright (c) 2014 Jae-jun Kang
// See the file COPYING for license details.

#ifndef X2BOOST_BUFFER_HPP_
#define X2BOOST_BUFFER_HPP_

#ifndef X2BOOST_PRE_HPP_
#include "x2boost/pre.hpp"
#endif

#include <deque>

namespace x2
{
    // A variable-length byte buffer class that provides methods to read/write
    // primitive data types from/to byte sequences in a platform-independent way.
    // The buffer capacity is limited to a multiple of a power of 2.
    // [TODO] consider block pooling
    class buffer
    {
    public:
        typedef boost::uint8_t byte;

        buffer() : buffer(12) {}

        explicit buffer(int exponent = 12)
        {
            if (exponent < 0 || 31 < exponent)
            {
                log::error() << "buffer block size exponent was " << exponent << std::endl;
                // exceptio
                return;
            }
            exponent_ = exponent;
            mask_ = ~(~0 << exponent);

            blocks_.push_back(new byte[block_size()]);

            block_index_ = 0;
            block_ = blocks_[block_index_];

            pos_ = back_ = front_ = 0;
            marker_ = -1;
        }
        ~buffer()
        {
            cleanup();
        }

        //void list_available_buffers(std::vector<boost::asio::mutable_buffer>& result)
        //{
            
        //}

        bool empty() const { return (back_ == front_); }
        std::size_t length() const { return (back_ - front_); }
        std::size_t pos() const { return pos_; }
        void set_pos(std::size_t value)
        {
            if (value < front_ || back_ < value)
            {
                log::error() << "buffer::set_pos index of out of bound" << std::endl;
                return;
            }
            pos_ = value;
            int block_index = pos_ >> exponent_;
            if ((block_index != 0) && ((pos_ & mask_) == 0))
            {
                --block_index;
            }
            if (block_index != block_index_)
            {
                block_index_ = block_index;
                block_ = blocks_[block_index_];
            }
        }

        void read_fixed(boost::int32_t& value)
        {
            check_length_to_read(4);
            value = read_byte();
            value = (value << 8) | read_byte();
            value = (value << 8) | read_byte();
            value = (value << 8) | read_byte();
        }

        void write_fixed(boost::int32_t& value)
        {
            ensure_capacity_to_write(4);
            write_byte((byte)(value >> 24));
            write_byte((byte)(value >> 16));
            write_byte((byte)(value >> 8));
            write_byte((byte)value);
        }

    private:
        void block_feed()
        {
            if ((pos_ & mask_) == 0 && (pos_ & ~mask_) != 0)
            {
                block_ = blocks_[++block_index_];
            }
        }

        void check_length_to_read(std::size_t n)
        {
            int limit = (marker_ >= 0 ? marker_ : back_);
            if ((pos_ + n) > limit)
            {
                // error
            }
        }

        void ensure_capacity_to_write(int n)
        {
            int required = pos_ + n;
            while (required >= capacity())
            {
                blocks_.push_back(new byte[block_size()]);
            }
            if (required > back_)
            {
                back_ = required;
            }
        }

        void cleanup()
        {
            if (blocks_.empty())
            {
                return;
            }
            for (std::size_t i = 0, count = blocks_.size(); i < count; ++i)
            {
                delete[] blocks_[i];
            }
            blocks_.clear();
            block_ = NULL;
        }

        byte read_byte()
        {
            block_feed();
            return block_[pos_++ & mask_];
        }

        void write_byte(byte value)
        {
            block_feed();
            block_[pos_++ & mask_] = value;
        }

        std::size_t block_size() { return ((std::size_t)1 << exponent_); }
        std::size_t capacity() { return (block_size() * blocks_.size()); }

        std::deque<byte*> blocks_;
        int exponent_;
        std::size_t mask_;

        byte* block_;
        int block_index_;

        std::size_t pos_, back_, front_;
        int marker_;
    };
}

#endif  // X2BOOST_BUFFER_HPP_
