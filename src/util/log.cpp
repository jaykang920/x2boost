// Copyright (c) 2014-2015 Jae-jun Kang
// See the file LICENSE for details.

#include "x2boost/util/log.hpp"

#include <cstdarg>
#include <cstdio>

#include <boost/algorithm/string.hpp>

using namespace x2;

int log::level = log_level::debug;
log::handler_func log::handler = NULL;

boost::thread_specific_ptr<log_stream> log::trace_;
boost::thread_specific_ptr<log_stream> log::debug_;
boost::thread_specific_ptr<log_stream> log::info_;
boost::thread_specific_ptr<log_stream> log::warning_;
boost::thread_specific_ptr<log_stream> log::error_;

namespace
{
    void emitf(int log_level, const char* format, va_list args)
    {
        const std::size_t length = 1024;
        char buffer[length + 1];
        ::vsnprintf(buffer, length, format, args);
        buffer[length] = '\0';
        log::emit(log_level, buffer);
    }
}

std::string log_level::string(int value)
{
    switch (value)
    {
    case all: return "all";
    case trace: return "trace";
    case debug: return "debug";
    case info: return "info";
    case warning: return "warning";
    case error: return "error";
    case none: return "none";
    default: return "unknown";
    }
}

int log_stream::buf::sync()
{
    if (log::handler && stream_->level_ >= log::level)
    {
        std::string s(str());
        str("");
        boost::trim_right(s);
        log::emit(stream_->level_, s);
    }
    return 0;
}

void log::tracef(const char* format, ...)
{
    if (!handler || level > log_level::trace) { return; }
    va_list args;
    va_start(args, format);
    emitf(log_level::trace, format, args);
    va_end(args);
}

void log::debugf(const char* format, ...)
{
    if (!handler || level > log_level::debug) { return; }
    va_list args;
    va_start(args, format);
    emitf(log_level::debug, format, args);
    va_end(args);
}

void log::infof(const char* format, ...)
{
    if (!handler || level > log_level::info) { return; }
    va_list args;
    va_start(args, format);
    emitf(log_level::info, format, args);
    va_end(args);
}

void log::warnf(const char* format, ...)
{
    if (!handler || level > log_level::warning) { return; }
    va_list args;
    va_start(args, format);
    emitf(log_level::warning, format, args);
    va_end(args);
}

void log::errorf(const char* format, ...)
{
    if (!handler || level > log_level::error) { return; }
    va_list args;
    va_start(args, format);
    emitf(log_level::error, format, args);
    va_end(args);
}

// EOF log.cpp
