// Copyright (c) 2014-2016 Jae-jun Kang
// See the file LICENSE for details.

#include <boost/test/unit_test.hpp>

#include <x2boost/buffer.hpp>

using namespace x2;

BOOST_AUTO_TEST_SUITE(buffer_test)

BOOST_AUTO_TEST_CASE(buffer_test_ctor)
{
    buffer b(4);

    BOOST_CHECK_EQUAL(16, b.block_size());
    BOOST_CHECK_EQUAL(16, b.capacity());
    BOOST_CHECK_EQUAL(true, b.empty());
    BOOST_CHECK_EQUAL(0, b.length());
    BOOST_CHECK_EQUAL(0, b.pos());
}

BOOST_AUTO_TEST_SUITE_END()

// EOF buffer_test.cpp
