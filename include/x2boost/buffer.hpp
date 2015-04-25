// Copyright (c) 2014-2015 Jae-jun Kang
// See the file LICENSE for details.

#ifndef X2BOOST_BUFFER_HPP_
#define X2BOOST_BUFFER_HPP_

#ifndef X2BOOST_PRE_HPP_
#include "x2boost/pre.hpp"
#endif

#include <deque>

namespace x2
{
    // A variable-length byte buffer class whose capacity is limited to a
    // multiple of a power of 2.
    class buffer
    {
    public:
        explicit buffer(int block_size_exponent = 12);
        ~buffer() { cleanup(); }

        // Gets the block size in bytes.
        int block_size() const { return (1 << block_size_exponent_); }

        // Gets the maximum capacity of the buffer.
        int capacity() const { return (int)(block_size() * blocks_.size()); }

        // Checks whether the buffer is empty (i.e. whether its length is 0).
        bool empty() const { return (back_ == front_); }

        // Gets the length of the buffered bytes.
        int length() const { return (back_ - front_); }

        // Gets the current zero-based position.
        int pos() const { return (pos_ - front_); }

        // Sets the current zero-based position.
        void pos(int value);

        //void list_available_buffers(std::vector<boost::asio::mutable_buffer>& result)
        //{
            
        //}

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
            write_byte((byte_t)(value >> 24));
            write_byte((byte_t)(value >> 16));
            write_byte((byte_t)(value >> 8));
            write_byte((byte_t)value);
        }

    private:
        void block_feed()
        {
            if ((pos_ & remainder_mask_) == 0 && (pos_ & ~remainder_mask_) != 0)
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

        void cleanup();
        void ensure_capacity_to_write(int n);

        byte_t read_byte()
        {
            block_feed();
            return block_[pos_++ & remainder_mask_];
        }

        void write_byte(byte_t value)
        {
            block_feed();
            block_[pos_++ & remainder_mask_] = value;
        }

    private:
        std::deque<byte_t*> blocks_;

        int block_size_exponent_;
        int remainder_mask_;

        byte_t* block_;
        int block_index_;

        int pos_, back_, front_;
        int marker_;
    };
}

#endif  // X2BOOST_BUFFER_HPP_
