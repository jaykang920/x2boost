// Copyright (c) 2014 Jae-jun Kang
// See the file COPYING for license details.

#ifndef X2BOOST_HANDLER_HPP_
#define X2BOOST_HANDLER_HPP_

#ifndef X2BOOST_PRE_HPP_
#include "x2boost/pre.hpp"
#endif

namespace x2
{
    struct X2BOOST_API handler
    {
        virtual ~handler() {}

        virtual void invoke(event_ptr e) = 0;

    protected:
        handler() {}
    };

    typedef std::vector<handler_ptr> handler_chain_type;

    template<class T, class E>
    class mem_fun_ptr_handler : public handler
    {
    public:
        mem_fun_ptr_handler(T* target, void (T::*f)(boost::shared_ptr<E>)) :
            f_(f),
            target_(target)
        {}

        virtual void invoke(event_ptr e)
        {
            (target_->*f_)(boost::static_pointer_cast<E>(e));
        }

    private:
        void (T::*f_)(boost::shared_ptr<E>);
        T* target_;
    };
}

#endif  // X2BOOST_HANDLER_HPP_
