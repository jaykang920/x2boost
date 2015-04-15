// Copyright (c) 2014-2015 Jae-jun Kang
// See the file LICENSE for details.

#ifndef X2BOOST_LOCKFREE_QUEUE_HPP_
#define X2BOOST_LOCKFREE_QUEUE_HPP_

#ifndef X2BOOST_PRE_HPP_
#include "x2boost/pre.hpp"
#endif

#include <deque>

#include <boost/lockfree/queue.hpp>
#include <boost/thread.hpp>

#include "x2boost/queue.hpp"

namespace x2
{
    template<typename T>
    class X2BOOST_API lockfree_queue : public queue_interface<boost::shared_ptr<T>>
    {
    public:
        lockfree_queue() : closing_(false) {}

        virtual ~lockfree_queue() {}

        virtual void close()
        {
            if (closing_) { return; }
            closing_ = true;
        }

        virtual boost::shared_ptr<T> dequeue()
        {
            boost::shared_ptr<T> value;
            boost::shared_ptr<T>* p;
            while (!store_.pop(p))
            {
                if (closing_)
                {
                    return value;
                }
                boost::thread::yield();
            }
            value.swap(*p);
            delete p;
            return value;
        }

        virtual bool enqueue(boost::shared_ptr<T> value)
        {
            boost::shared_ptr<T>* p = new boost::shared_ptr<T>(value);
            bool result = store_.push(p);
            if (!result)
            {
                delete p;
            }
            return result;
        }

        virtual bool try_dequeue(boost::shared_ptr<T>* value)
        {
            boost::shared_ptr<T>* p;
            bool result = store_.pop(p);
            if (result)
            {
                value->swap(*p);
                delete p;
            }
            return result;
        }

        virtual size_t size() const { return store_.size(); }

    protected:
        boost::lockfree::queue<T*> store_;
        volatile bool closing_;
    };

    typedef lockfree_queue<event> lockfree_event_queue;
}

#endif  // X2BOOST_LOCKFREE_QUEUE_HPP_
