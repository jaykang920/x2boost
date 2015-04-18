// Copyright (c) 2014-2015 Jae-jun Kang
// See the file LICENSE for details.

#ifndef X2BOOST_SHARED_OBJECT_HPP_
#define X2BOOST_SHARED_OBJECT_HPP_

#ifndef X2BOOST_PRE_HPP_
#include "x2boost/pre.hpp"
#endif

#include <boost/atomic.hpp>

namespace x2
{
    // Provides intrusive reference counting for boost::intrusive_ptr<>
    class X2BOOST_API shared_object
    {
    public:
        virtual ~shared_object() {}

    protected:
        shared_object() : ref_count_(0) {}
    
    private:
        // Increments the object reference count.
        friend void intrusive_ptr_add_ref(const shared_object* x)
        {
            x->ref_count_.fetch_add(1, boost::memory_order_relaxed);
        }

        // Destroys the object when its reference count drops to zero.
        friend void intrusive_ptr_release(const shared_object* x)
        {
            if (x->ref_count_.fetch_sub(1, boost::memory_order_release) == 1)
            {
                boost::atomic_thread_fence(boost::memory_order_acquire);
                delete x;
            }
        }

        mutable boost::atomic<int> ref_count_;
    };
}

#endif  // X2BOOST_SHARED_OBJECT_HPP_
