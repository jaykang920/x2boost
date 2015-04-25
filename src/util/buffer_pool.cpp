// Copyright (c) 2014-2015 Jae-jun Kang
// See the file LICENSE for details.

#include "x2boost/util/buffer_pool.hpp"

#include <boost/pool/singleton_pool.hpp>

using namespace x2;

void* buffer_pool::acquire(int size_exponent)
{
    switch (size_exponent)
    {
    case 4:
        return boost::singleton_pool<buffer_pool, 1 << 4>::malloc();
    case 5:
        return boost::singleton_pool<buffer_pool, 1 << 5>::malloc();
    case 6:
        return boost::singleton_pool<buffer_pool, 1 << 6>::malloc();
    case 7:
        return boost::singleton_pool<buffer_pool, 1 << 7>::malloc();
    case 8:
        return boost::singleton_pool<buffer_pool, 1 << 8>::malloc();
    case 9:
        return boost::singleton_pool<buffer_pool, 1 << 9>::malloc();
    case 10:
        return boost::singleton_pool<buffer_pool, 1 << 10>::malloc();
    case 11:
        return boost::singleton_pool<buffer_pool, 1 << 11>::malloc();
    case 12:
        return boost::singleton_pool<buffer_pool, 1 << 12>::malloc();
    case 13:
        return boost::singleton_pool<buffer_pool, 1 << 13>::malloc();
    case 14:
        return boost::singleton_pool<buffer_pool, 1 << 14>::malloc();
    case 15:
        return boost::singleton_pool<buffer_pool, 1 << 15>::malloc();
    case 16:
        return boost::singleton_pool<buffer_pool, 1 << 16>::malloc();
    case 17:
        return boost::singleton_pool<buffer_pool, 1 << 17>::malloc();
    case 18:
        return boost::singleton_pool<buffer_pool, 1 << 18>::malloc();
    case 19:
        return boost::singleton_pool<buffer_pool, 1 << 19>::malloc();
    case 20:
        return boost::singleton_pool<buffer_pool, 1 << 20>::malloc();
    default:
        break;
    }
    return 0;
}

void buffer_pool::release(int size_exponent, void* p)
{
    switch (size_exponent)
    {
    case 4:
        boost::singleton_pool<buffer_pool, 1 << 4>::free(p);
        break;
    case 5:
        boost::singleton_pool<buffer_pool, 1 << 5>::free(p);
        break;
    case 6:
        boost::singleton_pool<buffer_pool, 1 << 6>::free(p);
        break;
    case 7:
        boost::singleton_pool<buffer_pool, 1 << 7>::free(p);
        break;
    case 8:
        boost::singleton_pool<buffer_pool, 1 << 8>::free(p);
        break;
    case 9:
        boost::singleton_pool<buffer_pool, 1 << 9>::free(p);
        break;
    case 10:
        boost::singleton_pool<buffer_pool, 1 << 10>::free(p);
        break;
    case 11:
        boost::singleton_pool<buffer_pool, 1 << 11>::free(p);
        break;
    case 12:
        boost::singleton_pool<buffer_pool, 1 << 12>::free(p);
        break;
    case 13:
        boost::singleton_pool<buffer_pool, 1 << 13>::free(p);
        break;
    case 14:
        boost::singleton_pool<buffer_pool, 1 << 14>::free(p);
        break;
    case 15:
        boost::singleton_pool<buffer_pool, 1 << 15>::free(p);
        break;
    case 16:
        boost::singleton_pool<buffer_pool, 1 << 16>::free(p);
        break;
    case 17:
        boost::singleton_pool<buffer_pool, 1 << 17>::free(p);
        break;
    case 18:
        boost::singleton_pool<buffer_pool, 1 << 18>::free(p);
        break;
    case 19:
        boost::singleton_pool<buffer_pool, 1 << 19>::free(p);
        break;
    case 20:
        boost::singleton_pool<buffer_pool, 1 << 20>::free(p);
        break;
    default:
        break;
    }
}

void buffer_pool::release_memory()
{
    boost::singleton_pool<buffer_pool, 1 << 4>::release_memory();
    boost::singleton_pool<buffer_pool, 1 << 5>::release_memory();
    boost::singleton_pool<buffer_pool, 1 << 6>::release_memory();
    boost::singleton_pool<buffer_pool, 1 << 7>::release_memory();
    boost::singleton_pool<buffer_pool, 1 << 8>::release_memory();
    boost::singleton_pool<buffer_pool, 1 << 9>::release_memory();
    boost::singleton_pool<buffer_pool, 1 << 10>::release_memory();
    boost::singleton_pool<buffer_pool, 1 << 11>::release_memory();
    boost::singleton_pool<buffer_pool, 1 << 12>::release_memory();
    boost::singleton_pool<buffer_pool, 1 << 13>::release_memory();
    boost::singleton_pool<buffer_pool, 1 << 14>::release_memory();
    boost::singleton_pool<buffer_pool, 1 << 15>::release_memory();
    boost::singleton_pool<buffer_pool, 1 << 16>::release_memory();
    boost::singleton_pool<buffer_pool, 1 << 17>::release_memory();
    boost::singleton_pool<buffer_pool, 1 << 18>::release_memory();
    boost::singleton_pool<buffer_pool, 1 << 19>::release_memory();
    boost::singleton_pool<buffer_pool, 1 << 20>::release_memory();
}

void buffer_pool::purge_memory()
{
    boost::singleton_pool<buffer_pool, 1 << 4>::purge_memory();
    boost::singleton_pool<buffer_pool, 1 << 5>::purge_memory();
    boost::singleton_pool<buffer_pool, 1 << 6>::purge_memory();
    boost::singleton_pool<buffer_pool, 1 << 7>::purge_memory();
    boost::singleton_pool<buffer_pool, 1 << 8>::purge_memory();
    boost::singleton_pool<buffer_pool, 1 << 9>::purge_memory();
    boost::singleton_pool<buffer_pool, 1 << 10>::purge_memory();
    boost::singleton_pool<buffer_pool, 1 << 11>::purge_memory();
    boost::singleton_pool<buffer_pool, 1 << 12>::purge_memory();
    boost::singleton_pool<buffer_pool, 1 << 13>::purge_memory();
    boost::singleton_pool<buffer_pool, 1 << 14>::purge_memory();
    boost::singleton_pool<buffer_pool, 1 << 15>::purge_memory();
    boost::singleton_pool<buffer_pool, 1 << 16>::purge_memory();
    boost::singleton_pool<buffer_pool, 1 << 17>::purge_memory();
    boost::singleton_pool<buffer_pool, 1 << 18>::purge_memory();
    boost::singleton_pool<buffer_pool, 1 << 19>::purge_memory();
    boost::singleton_pool<buffer_pool, 1 << 20>::purge_memory();
}

// EOF buffer_pool.cpp
