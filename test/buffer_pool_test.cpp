// Copyright (c) 2014-2017 Jae-jun Kang
// See the file LICENSE for details.

#include <boost/test/unit_test.hpp>

#include <x2boost/util/buffer_pool.hpp>

using namespace x2boost;

BOOST_AUTO_TEST_SUITE(buffer_pool_test)

BOOST_AUTO_TEST_CASE(buffer_pool_test_acquire_release)
{
    byte_t* p;

    p = buffer_pool::acquire(4);
    BOOST_CHECK(p != NULL);
    buffer_pool::release(4, p);

    p = buffer_pool::acquire(12);
    BOOST_CHECK(p != NULL);
    buffer_pool::release(12, p);

    p = buffer_pool::acquire(20);
    BOOST_CHECK(p != NULL);
    buffer_pool::release(20, p);
}

BOOST_AUTO_TEST_CASE(buffer_pool_test_invalid_acquire)
{
    byte_t* p;
    
    p = buffer_pool::acquire(0);
    BOOST_CHECK(p == NULL);

    p = buffer_pool::acquire(32);
    BOOST_CHECK(p == NULL);
}

BOOST_AUTO_TEST_SUITE_END()

// EOF buffer_pool_test.cpp
