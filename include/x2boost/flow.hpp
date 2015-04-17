// Copyright (c) 2014-2015 Jae-jun Kang
// See the file LICENSE for details.

#ifndef X2BOOST_FLOW_HPP_
#define X2BOOST_FLOW_HPP_

#ifndef X2BOOST_PRE_HPP_
#include "x2boost/pre.hpp"
#endif

#include <boost/enable_shared_from_this.hpp>
#include <boost/thread/tss.hpp>

#include "x2boost/binder.hpp"
#include "x2boost/case.hpp"
#include "x2boost/handler.hpp"

namespace x2
{
    class X2BOOST_API flow :
        public boost::enable_shared_from_this<flow>,
        private boost::noncopyable
    {
    public:
        virtual ~flow() {}

        virtual void feed(event_ptr e) = 0;

        flow_ptr add(case_ptr c)
        {
            case_stack_.add(c);
            return shared_from_this();
        }

        flow_ptr remove(case_ptr c)
        {
            case_stack_.remove(c);
            return shared_from_this();
        }

        virtual void startup() {}
        virtual void shutdown()
        {
            std::cout << "flow::shutdown" << std::endl;
        }

        template<class E, class T>
        static void bind(boost::shared_ptr<E> e, void (T::*mf)(boost::shared_ptr<E>), T* t)
        {
            current_flow_->subscribe(e, mf, t);
        }

        template<class E, class T>
        flow_ptr subscribe(boost::shared_ptr<E> e, void (T::*mf)(boost::shared_ptr<E>), T* t)
        {
            handler* h = new mem_fun_ptr_handler<T, E>(t, mf);
            binder_.bind(e, handler_ptr(h));
            return shared_from_this();
        }

        /// Makes this flow subscribe to the specified channel.
        void subscribe_to(const char* channel) const;
        /// Makes this flow unsubscribe from the specified channel.
        void unsubscribe_from(const char* channel) const;

        static boost::thread_specific_ptr<flow>& current_flow() { return current_flow_; }

    protected:
        virtual void setup()
        {

        }
        virtual void teardown() {}

        void dispatch(event_ptr e);

        static boost::thread_specific_ptr<flow> current_flow_;
        static boost::thread_specific_ptr<handler_chain_type> handler_chain_;

        binder binder_;
        case_stack case_stack_;

        mutable boost::mutex mutex_;
    };
}

#endif  // X2BOOST_FLOW_HPP_
