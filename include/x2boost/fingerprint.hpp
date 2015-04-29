// Copyright (c) 2014-2015 Jae-jun Kang
// See the file LICENSE for details.

#ifndef X2BOOST_FINGERPRINT_HPP_
#define X2BOOST_FINGERPRINT_HPP_

#ifndef X2BOOST_PRE_HPP_
#include "x2boost/pre.hpp"
#endif

namespace x2
{
    class deserializer;
    class serializer;

    // Manages a fixed-length compact array of bit values.
    class X2BOOST_API fingerprint
    {
    public:
        // Initializes a new fingerprint object that can hold the specified
        // number of bit values, which are initially set to false.
        explicit fingerprint(std::size_t length);
        // Initializes a new fingerprint object that contains bit values copied
        // from the specified fingerprint.
        fingerprint(const fingerprint& other);

        virtual ~fingerprint();

        // Clears all the bits in the fingerprint, setting them as false.
        void clear();

        // Gets the bit value at the specified index.
        bool get(std::size_t index) const
        {
            if (index >= length_) { return false; }
            if ((index & ~block_mask) != 0)
            {
                index -= bits_per_block;
                return ((blocks_[index >> block_exp] & ((std::size_t)1 << (index & block_mask))) != 0);
            }
            return ((block_ & ((std::size_t)1 << index)) != 0);
        }
        // Sets the bit at the specified index.
        void touch(std::size_t index)
        {
            if (index >= length_) { return; }
            if ((index & ~block_mask) != 0)
            {
                index -= bits_per_block;
                blocks_[index >> block_exp] |= ((std::size_t)1 << (index & block_mask));
            }
            block_ |= ((std::size_t)1 << index);
        }
        // Clears the bit at the specified index.
        void wipe(std::size_t index)
        {
            if (index >= length_) { return; }
            if ((index & ~block_mask) != 0)
            {
                index -= bits_per_block;
                blocks_[index >> block_exp] &= ~(1 << (index & block_mask));
            }
            block_ &= ~((std::size_t)1 << index);
        }

        // Determines whether this fingerprint is equivalent to the specified one.
        bool equivalent(const fingerprint& other) const;
        // Returns the hash code for the current object.
        std::size_t hash_code() const;
        // Gets the number of effective bits in this fingerprint.
        std::size_t length() const { return length_; }

        void deserialize(deserializer& deserializer);
        int get_encoded_length() const;
        void serialize(serializer& serializer) const;

        bool operator[](std::size_t index) const
        {
            return get(index);
        }

        // The copy assignment operator.
        fingerprint& operator=(const fingerprint& other)
        {
            if (this != &other)
            {
                fingerprint temp(other);
                swap(temp);
            }
            return *this;
        }
        // Determines whether this fingerprint is less than the specified one.
        bool operator<(const fingerprint& other) const;
        // Determines whether this fingerprint is equal to the specified one.
        bool operator==(const fingerprint& other) const;
        // Determines whether this fingerprint is not equal to the specified one.
        bool operator!=(const fingerprint& other) const
        {
            return !(*this == other);
        }

    protected:
        // Exchanges the values of this object and the specified one.
        void swap(fingerprint& other)
        {
            std::swap(length_, other.length_);
            std::swap(block_, other.block_);
            std::swap(blocks_, other.blocks_);
        }

    private:
        static const std::size_t bits_per_byte = 8;
        static const std::size_t byte_exp = 3;
        static const std::size_t bits_per_block = sizeof(size_t) * bits_per_byte;
        static const std::size_t block_exp = (bits_per_block == 64 ? 6 : 5);
        static const std::size_t block_mask = bits_per_block - 1;

        // Gets the minimum number of bytes required to hold all the effective
        // bits in this fingerprint.
        int length_in_bytes() const
        {
            return (length_ == 0 ? 0 :
                ((length_ - 1) >> byte_exp) + 1);
        }

        // Gets the minimum number of additional blocks required to hold all
        // the effective bits in this fingerprint.
        int num_additional_blocks() const
        {
            return (length_ <= bits_per_block ? 0 :
                ((length_ - (bits_per_block + 1)) >> block_exp) + 1);
        }

        boost::int32_t length_;
        std::size_t block_;
        std::size_t* blocks_;
    };

    // Extends fingerprint class to hold an additional reference count.
    class slot : public fingerprint
    {
    public:
        slot(const fingerprint& base) : fingerprint(base), ref_count_(1) {}
        slot(const slot& other) : fingerprint(other), ref_count_(other.ref_count_) {}
        virtual ~slot() {}

        // Increases the reference count of this slot.
        void add_ref() { ++ref_count_; }
        // Decreases the reference count of this slot.
        int remove_ref() { return --ref_count_; }

        // The copy assignment operator.
        slot& operator=(const slot& other)
        {
            if (this != &other)
            {
                slot temp(other);
                swap(temp);
            }
            return *this;
        }

    protected:
        // Exchanges the values of this object and the specified one.
        void swap(slot& other)
        {
            fingerprint::swap(other);
            std::swap(ref_count_, other.ref_count_);
        }
    
    private:
        int ref_count_;
    };

    class X2BOOST_API capo : private boost::noncopyable
    {
    public:
        capo(const fingerprint& fp, int offset) : fp_(fp), offset_(offset) {}

        bool operator[](std::size_t index) const
        {
            return fp_.get(index + offset_);
        }
    private:
        const fingerprint& fp_;
        int offset_;
    };
}

#endif  // X2BOOST_FINGERPRINT_HPP_
