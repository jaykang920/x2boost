// Copyright (c) 2014-2017 Jae-jun Kang
// See the file LICENSE for details.

#ifndef X2BOOST_SYNCHRONIZED_EVENT_QUEUE_HPP_
#define X2BOOST_SYNCHRONIZED_EVENT_QUEUE_HPP_

#ifndef X2BOOST_PRE_HPP_
#include "x2boost/pre.hpp"
#endif

#include <queue>

#include <boost/thread/locks.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>

#include "x2boost/event_queue.hpp"

namespace x2boost
{
    class X2BOOST_API synchronized_event_queue : public event_queue
    {
    public:
        synchronized_event_queue() : closing_(false) {}

        virtual ~synchronized_event_queue() {}

        virtual void close()
        {
            boost::mutex::scoped_lock lock(mutex_);
            if (closing_) { return; }
            closing_ = true;
            cond_.notify_all();
        }

        virtual event_ptr dequeue()
        {
            event_ptr value;
            boost::mutex::scoped_lock lock(mutex_);
            while (store_.empty())
            {
                if (closing_)
                {
                    return value;
                }
                cond_.wait(lock);
            }
            value = store_.front();
            store_.pop();
            return value;
        }

        virtual bool enqueue(const event_ptr& value)
        {
            boost::mutex::scoped_lock lock(mutex_);
            store_.push(value);
            if (store_.size() == 1)
            {
                cond_.notify_one();
            }
            return true;
        }

        virtual bool try_dequeue(event_ptr* value)
        {
            boost::mutex::scoped_lock lock(mutex_);
            if (store_.empty())
            {
                event_ptr empty;
                *value = empty;
                return false;
            }
            *value = store_.front();
            store_.pop();
            return true;
        }

        virtual size_t size() const { return store_.size(); }

    protected:
        std::queue<event_ptr> store_;
        volatile bool closing_;

        mutable boost::mutex mutex_;
        mutable boost::condition_variable cond_;
    };
}

#endif  // X2BOOST_SYNCHRONIZED_EVENT_QUEUE_HPP_
