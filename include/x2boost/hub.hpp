// Copyright (c) 2014 Jae-jun Kang
// See the file COPYING for license details.

#ifndef X2BOOST_HUB_HPP_
#define X2BOOST_HUB_HPP_

#ifndef X2BOOST_PRE_HPP_
#include "x2boost/pre.hpp"
#endif

namespace x2
{
    /// Represents the singleton event distribution bus.
    X2BOOST_API
    class Hub : private boost::noncopyable
    {
    public:
        static Hub& get_instance() { return instance_; }

    private:
        static Hub instance_;

        std::vector<FlowPtr> attached_;
    };
}

#endif  // X2BOOST_HUB_HPP_
