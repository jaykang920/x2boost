// Copyright (c) 2014-2016 Jae-jun Kang
// See the file LICENSE for details.

#ifndef X2BOOST_PRE_HPP_
#define X2BOOST_PRE_HPP_

#ifdef X2_AS_SHARED_LIB
#if defined(_WIN32)
#if defined(X2BOOST_EXPORTS)
#define X2BOOST_API __declspec(dllexport)
#else
#define X2BOOST_API __declspec(dllimport)
#endif
#endif
#endif  // X2_AS_SHARED_LIB

#ifndef X2BOOST_API
#define X2BOOST_API
#endif

#include <string>
#include <vector>

#include <boost/cstdint.hpp>
#include <boost/intrusive_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/version.hpp>

#include "x2boost/util/log.hpp"

namespace x2
{
    struct case_interface;
    class cell;
    class event;
    class flow;
    struct handler;

    typedef boost::shared_ptr<case_interface> case_ptr;
    typedef boost::intrusive_ptr<cell> cell_ptr;
    typedef boost::intrusive_ptr<event> event_ptr;
    typedef boost::shared_ptr<flow> flow_ptr;
    typedef boost::intrusive_ptr<handler> handler_ptr;

    typedef boost::uint8_t byte_t;
}

#endif  // X2BOOST_PRE_HPP_
