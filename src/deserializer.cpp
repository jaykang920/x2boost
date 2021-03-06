// Copyright (c) 2014-2017 Jae-jun Kang
// See the file LICENSE for details.

#include "x2boost/deserializer.hpp"

using namespace x2boost;

void deserializer::read(std::string& value)
{
    value.clear();
    boost::int32_t length;
    read_nonnegative(length);
    if (length == 0)
    {
        return;
    }
    buffer_.check_length_to_read(length);

    std::ostringstream oss;
    for (int i = 0; i < length; ++i)
    {
        oss << buffer_.get_byte();
    }
    value.assign(oss.str());
}

// EOF deserializer.cpp
