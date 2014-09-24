// Copyright (c) 2014 Jae-jun Kang
// See the file COPYING for license details.

#ifndef X2BOOST_SYNCHRONIZED_QUEUE_HPP_
#define X2BOOST_SYNCHRONIZED_QUEUE_HPP_

#ifndef X2BOOST_PRE_HPP_
#include "x2boost/pre.hpp"
#endif

#include <deque>

#include <boost/thread/locks.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>

#include "x2boost/queue.hpp"

namespace x2
{
    template<typename T>
    class X2BOOST_API synchronized_queue : public queue_interface<T>
    {
    public:
        synchronized_queue() : closing_(false) {}

        virtual ~synchronized_queue() {}

        virtual void close()
        {
            if (closing_) { return; }
            closing_ = true;
            cond_.notify_all();
        }

        virtual T dequeue()
        {
            T value;
            boost::mutex::scoped_lock lock(mutex_);
            if (store_.empty())
            {
                if (closing_)
                {
                    return value;
                }
                cond_.wait(lock);
            }
            value = store_.front();
            store_.pop_front();
            return value;
        }

        virtual bool enqueue(T value)
        {
            boost::mutex::scoped_lock lock(mutex_);
            store_.push_back(value);
            cond_.notify_one();
            return true;
        }

        virtual bool try_dequeue(T* value)
        {
            boost::mutex::scoped_lock lock(mutex_);
            if (store_.empty())
            {
                T empty;
                *value = empty;
                return false;
            }
            *value = store_.front();
            store_.pop_front();
            return true;
        }

        virtual size_t size() const { return store_.size(); }

    protected:
        std::deque<T> store_;
        volatile bool closing_;

        mutable boost::mutex mutex_;
        mutable boost::condition_variable cond_;
    };
}

#endif  // X2BOOST_SYNCHRONIZED_QUEUE_HPP_
