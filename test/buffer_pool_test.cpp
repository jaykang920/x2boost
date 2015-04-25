// Copyright (c) 2014-2015 Jae-jun Kang
// See the file LICENSE for details.

#include <boost/test/unit_test.hpp>

#include <x2boost/util/buffer_pool.hpp>

using namespace x2;

BOOST_AUTO_TEST_SUITE(buffer_pool_test)

BOOST_AUTO_TEST_CASE(buffer_pool_test)
{
    void* p;

    p = buffer_pool::acquire(4);
    BOOST_CHECK(p != 0);
    buffer_pool::release(4, p);

    p = buffer_pool::acquire(12);
    BOOST_CHECK(p != 0);
    buffer_pool::release(12, p);

    p = buffer_pool::acquire(20);
    BOOST_CHECK(p != 0);
    buffer_pool::release(20, p);

    buffer_pool::release_memory();
    buffer_pool::purge_memory();
}

BOOST_AUTO_TEST_SUITE_END()

// EOF buffer_pool_test.cpp
