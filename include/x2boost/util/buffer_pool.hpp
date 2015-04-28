// Copyright (c) 2014-2015 Jae-jun Kang
// See the file LICENSE for details.

#ifndef X2BOOST_BUFFER_POOL_HPP_
#define X2BOOST_BUFFER_POOL_HPP_

#ifndef X2BOOST_PRE_HPP_
#include "x2boost/pre.hpp"
#endif

namespace x2
{
    // Singleton pool of 2^n length byte buffer blocks.
    class X2BOOST_API buffer_pool
    {
    public:
        static const int max_size_exponent = 20;  // 1MB
        static const int min_size_exponent = 4;   // 16-byte

        // Acquires a byte buffer block of length 2^size_exponent from the pool.
        static byte_t* acquire(int size_exponent);
        // Releases the byte buffer block and pushes it back to the pool.
        static void release(int size_exponent, byte_t* p);

        // Releases every memory block that doesn't have any allocated chunks.
        static void release_memory();
        // Force to release every memory block.
        static void purge_memory();

    private:
        buffer_pool() { }
    };
}

#endif  // X2BOOST_BUFFER_POOL_HPP_
