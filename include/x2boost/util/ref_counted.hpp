// Copyright (c) 2014-2017 Jae-jun Kang
// See the file LICENSE for details.

#ifndef X2BOOST_REF_COUNTED_HPP_
#define X2BOOST_REF_COUNTED_HPP_

#ifndef X2BOOST_PRE_HPP_
#include "x2boost/pre.hpp"
#endif

#include <boost/atomic.hpp>

namespace x2boost
{
    /// Provides intrusive reference counting for boost::intrusive_ptr<>.
    class X2BOOST_API ref_counted
    {
    public:
        virtual ~ref_counted() { }

    protected:
        ref_counted() : ref_count_(0) { }
    
    private:
        // Increments the object reference count.
        friend void intrusive_ptr_add_ref(const ref_counted* x)
        {
            x->ref_count_.fetch_add(1, boost::memory_order_relaxed);
        }

        // Decrements the object reference count, and destroys the object when
        // its reference count drops to zero.
        friend void intrusive_ptr_release(const ref_counted* x)
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

#endif  // X2BOOST_REF_COUNTED_HPP_
