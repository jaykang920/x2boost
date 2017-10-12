// Copyright (c) 2014-2017 Jae-jun Kang
// See the file LICENSE for details.

#ifndef X2BOOST_RANGED_INT_POOL_HPP_
#define X2BOOST_RANGED_INT_POOL_HPP_

#ifndef X2BOOST_PRE_HPP_
#include "x2boost/pre.hpp"
#endif

#include <bitset>

namespace x2boost
{
    // Compact pool of consecutive integer values in a finite range.
    template<int Min, int Max>
    class X2BOOST_API ranged_int_pool
    {
    public:
        explicit ranged_int_pool(bool advancing = false)
            : offset_(0), advancing_(advancing) { }

        // Gets the next available value from the pool.
        int acquire()
        {
            for (int i = 0, index = offset_, l = length(); i < l; ++i, ++index)
            {
                if (index >= l)
                {
                    index = 0;
                }
                if (!bitset_[index])
                {
                    bitset_.set(index);
                    if (advancing_)
                    {
                        offset_ = index + 1;
                        if (offset_ > l)
                        {
                            offset_ = 0;
                        }
                    }
                    return (Min + index);
                }
            }
            return (Min - 1);  // our of resource
        }

        // Marks the specified value as used in the pool.
        bool claim(int value)
        {
            int index = value - Min;
            if (bitset_[index])
            {
                return false;
            }
            bitset_.set(index);
            return true;
        }

        // Gets the number of consecutive integers handled by this pool.
        int length() const { return (int)bitset_.size(); }

        // Returns the specified value to the pool.
        void release(int value)
        {
            int index = value - Min;
            if (bitset_[index])
            {
                bitset_.reset(index);
            }
        }

    private:
        std::bitset<Max - Min + 1> bitset_;
        int offset_;
        bool advancing_;
    };
}

#endif  // X2BOOST_RANGED_INT_POOL_HPP_
