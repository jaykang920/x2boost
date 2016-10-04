// Copyright (c) 2014-2016 Jae-jun Kang
// See the file LICENSE for details.

#include "x2boost/serializer.hpp"

using namespace x2;

void serializer::write(const std::string& value)
{
    boost::int32_t length = (boost::int32_t)value.length();
    write_variable_nonnegative(length);
    buffer_.ensure_capacity_to_write(length);
    for (int i = 0; i < length; ++i)
    {
        buffer_.put_byte((byte_t)value[i]);
    }
}

// EOF serializer.cpp
