// Copyright (c) 2014-2017 Jae-jun Kang
// See the file LICENSE for details.

#ifndef X2BOOST_BUFFER_HPP_
#define X2BOOST_BUFFER_HPP_

#ifndef X2BOOST_PRE_HPP_
#include "x2boost/pre.hpp"
#endif

#include <deque>

namespace boost { namespace asio {
    class const_buffer;
    class mutable_buffer;
} }

namespace x2boost
{
    // A variable-length byte buffer class whose capacity is limited to a
    // multiple of a power of 2.
    class X2BOOST_API buffer
    {
    public:
        explicit buffer(int block_size_exponent = 12);
        ~buffer() { cleanup(); }

        // Gets the block size in bytes.
        int block_size() const { return (1 << block_size_exponent_); }

        // Gets the maximum capacity of the buffer.
        int capacity() const { return (int)(block_size() * blocks_.size()); }

        void check_length_to_read(std::size_t n)
        {
            int limit = (marker_ >= 0 ? marker_ : back_);
            if ((pos_ + n) > limit)
            {
                // error
            }
        }

        // Checks whether the buffer is empty (i.e. whether its length is 0).
        bool empty() const { return (back_ == front_); }

        void ensure_capacity_to_write(int n);

        byte_t get_byte()
        {
            block_feed();
            return block_[pos_++ & remainder_mask_];
        }

        // Gets the length of the buffered bytes.
        int length() const { return (back_ - front_); }

        // Gets the current zero-based position.
        int pos() const { return (pos_ - front_); }

        // Sets the current zero-based position.
        void pos(int value);

        void list_occupied_buffers(std::vector<boost::asio::const_buffer>& result) const
        {
            list_const_buffers(result, front_, back_);
        }
        void list_starting_buffers(std::vector<boost::asio::const_buffer>& result, int length) const
        {
            list_const_buffers(result, front_, front_ + length);
        }
        void list_ending_buffers(std::vector<boost::asio::const_buffer>& result, int length) const
        {
            list_const_buffers(result, back_ - length, back_);
        }
        // Retrieves the occupied const buffers with the specified range.
        void list_const_buffers(std::vector<boost::asio::const_buffer>& result, int begin, int end) const;
        void list_const_buffers(std::vector<boost::asio::const_buffer>& result, int length) const
        {
            list_const_buffers(result, pos_, pos_ + length);
        }
        // Retrieves the available mutable buffers.
        void list_mutable_buffers(std::vector<boost::asio::mutable_buffer>& result);

        void mark_to_read(int length)
        {
            if ((front_ + length) > back_)
            {
                // exception
                return;
            }
            marker_ = front_ + length;
        }

        void put_byte(byte_t value)
        {
            block_feed();
            block_[pos_++ & remainder_mask_] = value;
        }

        // Reads a sequence of bytes from the buffer and advances the position.
        void read(byte_t* buffer, int offset, int count);

        // Alias of pos(0).
        void rewind() { pos(0); }

        void shrink(int length);
        void stretch(int length);

        byte_t* to_array() const;

        void trim();

        // Writes a sequence of bytes to the buffer and advances the position.
        void write(byte_t* buffer, int offset, int count);

        byte_t& operator[] (int index)
        {
            index += front_;
            return blocks_[index >> block_size_exponent_][index & remainder_mask_];
        }

        const byte_t& operator[] (int index) const
        {
            index += front_;
            return blocks_[index >> block_size_exponent_][index & remainder_mask_];
        }

    private:
        // Copies a sequence of bytes from the specified byte array segment
        // to the buffer.
        void copy_from(int pos, byte_t* buffer, int offset, int count);
        // Copies a sequence of bytes from the buffer
        // to the specified byte array segment.
        void copy_to(int pos, byte_t* buffer, int offset, int count) const;

        void block_feed()
        {
            if ((pos_ & remainder_mask_) == 0 && (pos_ & ~remainder_mask_) != 0)
            {
                block_ = blocks_[++block_index_];
            }
        }

        void cleanup();

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
