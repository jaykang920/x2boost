// Copyright (c) 2014-2015 Jae-jun Kang
// See the file LICENSE for details.

#include "x2boost/flow.hpp"

#include "x2boost/hub.hpp"

using namespace x2;

boost::thread_specific_ptr<flow> flow::current_flow_;
boost::thread_specific_ptr<handler_chain_type> flow::handler_chain_;

void flow::subscribe_to(const char* channel) const
{
    hub::instance().subscribe(boost::const_pointer_cast<flow>(shared_from_this()), channel);
}

void flow::unsubscribe_from(const char* channel) const
{
    hub::instance().unsubscribe(boost::const_pointer_cast<flow>(shared_from_this()), channel);
}

void flow::dispatch(event_ptr e)
{
    std::size_t chain_length = binder_.build_handler_chain(e, *handler_chain_);
    if (!chain_length)
    {
        return;
    }

    for (std::size_t i = 0, count = handler_chain_->size(); i < count; ++i)
    {
        handler_ptr handler = handler_chain_->at(i);
        try
        {
            handler->invoke(e);
        }
        catch (std::exception& e)
        {
            std::cerr << e.what() << std::endl;
        }
    }

    handler_chain_->clear();
}


// EOF flow.cpp
