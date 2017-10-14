// Copyright (c) 2014-2017 Jae-jun Kang
// See the file LICENSE for details.

#include "x2boost/links/asio_link.hpp"

#include <boost/atomic.hpp>
#include <boost/math/special_functions/round.hpp>
#include <boost/thread.hpp>

using namespace x2boost;

boost::asio::io_service asio_link::io_service_;

namespace
{
    volatile bool should_stop = false;
    boost::atomic<int> ref_count(0);
    boost::thread_group workers;

    std::auto_ptr<boost::asio::io_service::work> work(new boost::asio::io_service::work(asio_link::io_service()));

    void run()
    {
        trace::debug("asio_worker begin");

        while (!should_stop)
        {
            try
            {
                boost::system::error_code ec;
                asio_link::io_service().run(ec);
                if (ec)
                {
                    trace::error(ec.message());
                }
            }
            catch (std::exception& e)
            {
                trace::error(e.what());
            }
            boost::this_thread::sleep_for(boost::chrono::milliseconds(1));
        }

        trace::debug("asio_worker end");
    }

    void asio_start()
    {
        trace::info("asio_start");

        int n = (int)boost::thread::hardware_concurrency();
        n = (int)boost::math::round((double)n * 0.66);
        for (int i = 0; i < n; ++i)
        {
            workers.create_thread(run);
        }
    }

    void asio_stop()
    {
        should_stop = true;
        work.reset();
        asio_link::io_service().stop();  // TODO FIXME rely on socket close
        workers.join_all();

        trace::info("asio_stop");
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
