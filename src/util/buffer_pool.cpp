// Copyright (c) 2014-2016 Jae-jun Kang
// See the file LICENSE for details.

#include "x2boost/util/buffer_pool.hpp"

#include <boost/pool/pool.hpp>
#include <boost/thread/mutex.hpp>

using namespace x2;

namespace
{
    const int size = buffer_pool::max_size_exponent - buffer_pool::min_size_exponent + 1;
    boost::pool<>* pools[size];
    boost::mutex mutex;

    struct static_inirializer
    {
        static_inirializer()
        {
            boost::mutex::scoped_lock lock(mutex);
            for (int i = 0; i < size; ++i)
            {
                pools[i] = NULL;
            }
        }
        ~static_inirializer()
        {
            buffer_pool::purge_memory();
        }
    };
    static_inirializer static_init;
}

byte_t* buffer_pool::acquire(int size_exponent)
{
    if (size_exponent < min_size_exponent || max_size_exponent < size_exponent)
    {
        return NULL;
    }
    int index = size_exponent - min_size_exponent;
    boost::mutex::scoped_lock lock(mutex);
    boost::pool<>* pool = pools[index];
    if (pool == NULL)
    {
        pool = new boost::pool<>(1 << size_exponent);
        pools[index] = pool;
    }
    return static_cast<byte_t*>(pool->malloc());
}

void buffer_pool::release(int size_exponent, byte_t* p)
{
    if (size_exponent < min_size_exponent || max_size_exponent < size_exponent)
    {
        return;
    }
    if (p == NULL)
    {
        return;
    }
    try
    {
        boost::mutex::scoped_lock lock(mutex);
        boost::pool<>* pool = pools[size_exponent - min_size_exponent];
        if (pool == NULL)
        {
            return;
        }
        pool->free(p);
    }
    catch (...)
    {
    }
}

void buffer_pool::release_memory()
{
    boost::mutex::scoped_lock lock(mutex);
    for (int i = 0, count = max_size_exponent - min_size_exponent + 1; i < count; ++i)
    {
        boost::pool<>* pool = pools[i];
        if (pool == NULL) { continue; }
        pool->release_memory();
    }
}

void buffer_pool::purge_memory()
{
    boost::mutex::scoped_lock lock(mutex);
    for (int i = 0, count = max_size_exponent - min_size_exponent + 1; i < count; ++i)
    {
        boost::pool<>* pool = pools[i];
        if (pool == NULL) { continue; }
        pools[i] = NULL;
        pool->purge_memory();
        delete pool;
    }
}

// EOF buffer_pool.cpp
