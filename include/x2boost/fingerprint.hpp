// Copyright (c) 2014 Jae-jun Kang
// See the file COPYING for license details.

#ifndef X2BOOST_FINGERPRINT_HPP_
#define X2BOOST_FINGERPRINT_HPP_

#ifndef X2BOOST_PRE_HPP_
#include "x2boost/pre.hpp"
#endif

#include <bitset>

namespace x2
{
    // Manages a fixed-length compact array of bit values.
    class X2BOOST_API fingerprint
    {
    public:
        // Initializes a new fingerprint object that can hold the specified
        // number of bit values, which are initially set to false.
        explicit fingerprint(size_t length);
        // Initializes a new fingerprint object that contains bit values copied
        // from the specified fingerprint.
        fingerprint(const fingerprint& other);

        ~fingerprint();

        // Clears all the bits in the fingerprint, setting them as false.
        void clear();

        // Gets the bit value at the specified index.
        bool get(std::size_t index) const
        {
            if (index >= length_) { return false; }
            if (index >= bits_per_block)
            {
                index -= bits_per_block;
                return ((blocks_[index >> block_exp] & (1 << (index & block_mask))) != 0);
            }
            return (block_ & (1 << index) != 0);
        }
        // Sets the bit at the specified index.
        void touch(std::size_t index)
        {
            if (index >= length_) { return; }
            if (index >= bits_per_block)
            {
                index -= bits_per_block;
                blocks_[index >> block_exp] |= (1 << (index & block_mask));
            }
            block_ |= (1 << index);
        }
        // Clears the bit at the specified index.
        void wipe(std::size_t index)
        {
            if (index >= length_) { return; }
            if (index >= bits_per_block)
            {
                index -= bits_per_block;
                blocks_[index >> block_exp] &= ~(1 << (index & block_mask));
            }
            block_ &= ~(1 << index);
        }

        // Returns the hash code for the current object.
        size_t hash_code() const;
        // Gets the number of effective bits in this fingerprint.
        size_t length() const { return length_; }

        // Determines whether this fingerprint is less than the specified one.
        bool operator<(const fingerprint& other) const;
        // Determines whether this fingerprint is equal to the specified one.
        bool operator==(const fingerprint& other) const;

    private:
        static const size_t bits_per_byte = 8;
        static const size_t byte_exp = 3;
        static const size_t bits_per_block = sizeof(size_t) * bits_per_byte;
        static const size_t block_exp = (bits_per_block == 64 ? 6 : 5);
        static const size_t block_mask = bits_per_block - 1;

        // Gets the minimum number of bytes required to hold all the effective
        // bits in this fingerprint.
        size_t length_in_bytes() const
        {
            return (length_ == 0 ? 0 :
                ((length_ - 1) >> byte_exp) + 1);
        }

        // Gets the minimum number of additional blocks required to hold all
        // the effective bits in this fingerprint.
        size_t num_additional_blocks() const
        {
            return (length_ <= bits_per_block ? 0 :
                ((length_ - (bits_per_block + 1)) >> block_exp) + 1);
        }

        void swap(fingerprint& other);

        size_t length_;
        size_t block_;
        size_t* blocks_;
    };
}

#endif  // X2BOOST_FINGERPRINT_HPP_
