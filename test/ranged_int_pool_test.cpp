// Copyright (c) 2014-2017 Jae-jun Kang
// See the file LICENSE for details.

#include <boost/test/unit_test.hpp>

#include <x2boost/util/ranged_int_pool.hpp>

using namespace x2boost;

BOOST_AUTO_TEST_SUITE(ranged_int_pool_test)

BOOST_AUTO_TEST_CASE(ranged_int_pool_test_acquire_release)
{
    ranged_int_pool<1, 2> p;

    BOOST_CHECK(p.length() == 2);
    int i = p.acquire();
    BOOST_CHECK(i == 1);
    p.release(i);
    i = p.acquire();
    BOOST_CHECK(i == 1);
    i = p.acquire();
    BOOST_CHECK(i == 2);
    p.release(i);
    i = p.acquire();
    BOOST_CHECK(i == 2);
}

BOOST_AUTO_TEST_CASE(ranged_int_pool_test_circulation)
{
    ranged_int_pool<1, 2> p(true);

    BOOST_CHECK(p.length() == 2);
    int i = p.acquire();
    BOOST_CHECK(i == 1);
    p.release(i);
    i = p.acquire();
    BOOST_CHECK(i == 2);
    i = p.acquire();
    BOOST_CHECK(i == 1);
}

BOOST_AUTO_TEST_CASE(ranged_int_pool_test_out_of_resource)
{
    ranged_int_pool<1, 2> p;

    BOOST_CHECK(p.length() == 2);
    p.acquire();
    p.acquire();
    int i = p.acquire();
    BOOST_CHECK(i == (1 - 1));
}

BOOST_AUTO_TEST_SUITE_END()

// EOF ranged_int_pool_test.cpp
