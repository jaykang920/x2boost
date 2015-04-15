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
    // For boost::intrusive_ptr<>
    class shared_object
    {
    public:
        virtual ~shared_object() {}

    protected:
        shared_object() {}
    
    private:
        friend void intrusive_ptr_add_ref(const shared_object* x)
        {
            x->ref_count_.fetch_add(1, boost::memory_order_relaxed);
        }
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
