// Copyright (c) 2014-2015 Jae-jun Kang
// See the file LICENSE for details.

#include "x2boost/links/asio_link.hpp"

#include <boost/atomic.hpp>
#include <boost/thread.hpp>

using namespace x2;

boost::asio::io_service asio_link::io_service_;

namespace
{
    volatile bool should_stop = false;
    boost::atomic<int> ref_count;
    boost::thread* worker = NULL;
    //boost::thread_group workers;

    void work()
    {
        while (!should_stop)
        {
            try
            {
                asio_link::io_service().run();
            }
            catch (std::exception& e)
            {
                log::error(e.what());
            }
            boost::this_thread::sleep_for(boost::chrono::milliseconds(1));
        }
    }

    void asio_start()
    {
        worker = new boost::thread(work);
        /*
        int num_workers = boost::thread::hardware_concurrency();
        if (num_workers > 2) { num_workers /= 2; }

        for (int i = 0; i < num_workers; ++i)
        {
            workers.create_thread(work);
        }
        */
    }

    void asio_stop()
    {
        should_stop = true;
        asio_link::io_service().stop();
        //workers.join_all();
        worker->join();
        delete worker;
    }
}

void asio_link::start()
{
    if (ref_count.fetch_add(1, boost::memory_order_release) == 0)
    {
        boost::atomic_thread_fence(boost::memory_order_acquire);
        asio_start();
    }
}

void asio_link::stop()
{
    if (ref_count.fetch_sub(1, boost::memory_order_release) == 1)
    {
        boost::atomic_thread_fence(boost::memory_order_acquire);
        asio_stop();
    }
}

// EOF asio_link.cpp
