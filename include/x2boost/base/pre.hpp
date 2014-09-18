// Copyright (c) 2014 Jae-jun Kang
// See the file COPYING for license details.

#ifndef X2BOOST_BASE_PRE_HPP_
#define X2BOOST_BASE_PRE_HPP_

#ifdef X2_AS_SHARED_LIB
#if defined(_WIN32)
#if defined(X2BOOST_EXPORTS)
#define X2BOOST_API __declspec(dllexport)
#else
#define X2BOOST_API __declspec(dllimport)
#endif
#endif
#endif  // X2_AS_SHARED_LIB

#ifndef X2CPP_API
#define X2CPP_API
#endif

// A macro to disallow the copy constructor and operator= functions
// This should be used in the private section of a class declaration.
#define X2_NO_COPY(name) \
    name(const name&); \
    void operator=(const name&)

#endif  // X2BOOST_BASE_PRE_HPP_
