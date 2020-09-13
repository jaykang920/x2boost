// Copyright (c) 2014-2017 Jae-jun Kang
// See the file LICENSE for details.

#ifndef X2BOOST_STRING_HPP_
#define X2BOOST_STRING_HPP_

#ifndef X2BOOST_PRE_HPP_
#include "x2boost/pre.hpp"
#endif

//#include <iostream>

namespace x2boost
{
    template<class T>
    std::ostream& operator<<(std::ostream& out, const boost::intrusive_ptr<T>& ptr)
    {
        //out << c.real;
        //out << "+i" << c.imag << endl;
        return out;
    }

    template<class T>
    std::istream& operator>>(std::istream& in, boost::intrusive_ptr<T>& ptr)
    {
        //cout << "Enter Real Part ";
        //in >> c.real;
        //cout << "Enter Imaginary Part ";
        //in >> c.imag;
        return in;
    }

    std::ostream& operator<<(std::ostream& out, const std::vector<std::string>& list)
    {
        //out << c.real;
        //out << "+i" << c.imag << endl;
        return out;
    }

    std::istream& operator>>(std::istream& in, std::vector<std::string>& list)
    {
        //cout << "Enter Real Part ";
        //in >> c.real;
        //cout << "Enter Imaginary Part ";
        //in >> c.imag;
        return in;
    }

    template<typename T>
    std::ostream& operator<<(std::ostream& out, const std::vector<T>& list)
    {
        //out << c.real;
        //out << "+i" << c.imag << endl;
        return out;
    }

    template<typename T>
    std::istream& operator>>(std::istream& in, std::vector<T>& list)
    {
        //cout << "Enter Real Part ";
        //in >> c.real;
        //cout << "Enter Imaginary Part ";
        //in >> c.imag;
        return in;
    }

    /*
    template<class T>
    std::ostream& operator<<(std::ostream& out, std::vector<boost::intrusive_ptr<T>>& list)
    {
        //out << c.real;
        //out << "+i" << c.imag << endl;
        return out;
    }

    template<class T>
    std::istream& operator>>(std::istream& in, std::vector<boost::intrusive_ptr<T>>& list)
    {
        //cout << "Enter Real Part ";
        //in >> c.real;
        //cout << "Enter Imaginary Part ";
        //in >> c.imag;
        return in;
    }

    template<class T>
    std::string to_string(boost::intrusive_ptr<T>& ptr)
    {
        return "";
    }

    template<typename T>
    std::string to_string(std::vector<boost::intrusive_ptr<T>>& list)
    {
        return "";
    }
    */
}

#endif  // X2BOOST_STRING_HPP_
