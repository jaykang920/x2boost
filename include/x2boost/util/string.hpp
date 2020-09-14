// Copyright (c) 2014-2017 Jae-jun Kang
// See the file LICENSE for details.

#ifndef X2BOOST_STRING_HPP_
#define X2BOOST_STRING_HPP_

#ifndef X2BOOST_PRE_HPP_
#include "x2boost/pre.hpp"
#endif

namespace
{
    template<class T>
    std::ostream& operator<<(std::ostream& out, const boost::intrusive_ptr<T>& ptr)
    {
        //out << c.real;
        //out << "+i" << c.imag << endl;
        return out;
    }

    template<typename T>
    std::ostream& operator<<(std::ostream& out, const std::vector<T>& list)
    {
        //out << c.real;
        //out << "+i" << c.imag << endl;
        return out;
    }
}

#endif  // X2BOOST_STRING_HPP_
