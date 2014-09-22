// Copyright (c) 2014 Jae-jun Kang
// See the file COPYING for license details.

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

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

namespace x2
{
    class case_interface;
    class cell;
    class event;
    class flow;

    typedef boost::shared_ptr<case_interface> case_ptr;
    typedef boost::shared_ptr<cell> cell_ptr;
    typedef boost::shared_ptr<event> event_ptr;
    typedef boost::shared_ptr<flow> flow_ptr;
}

#endif  // X2BOOST_PRE_HPP_
