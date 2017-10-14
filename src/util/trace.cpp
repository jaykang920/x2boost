// Copyright (c) 2014-2017 Jae-jun Kang
// See the file LICENSE for details.

#include "x2boost/util/trace.hpp"

#include <cstdarg>
#include <cstdio>

#include <boost/algorithm/string.hpp>

using namespace x2boost;

int trace::level = trace_level::debug;
trace::handler_func trace::handler = NULL;

boost::thread_specific_ptr<trace_stream> trace::trace_;
boost::thread_specific_ptr<trace_stream> trace::debug_;
boost::thread_specific_ptr<trace_stream> trace::info_;
boost::thread_specific_ptr<trace_stream> trace::warning_;
boost::thread_specific_ptr<trace_stream> trace::error_;

namespace
{
    void emitf(int trace_level, const char* format, va_list args)
    {
        const std::size_t length = 1024;
        char buffer[length + 1];
        ::vsnprintf(buffer, length, format, args);
        buffer[length] = '\0';
        trace::emit(trace_level, buffer);
    }
}

std::string trace_level::string(int value)
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

int trace_stream::buf::sync()
{
    if (trace::handler && stream_->level_ >= trace::level)
    {
        std::string s(str());
        str("");
        boost::trim_right(s);
        trace::emit(stream_->level_, s);
    }
    return 0;
}

void trace::logf(const char* format, ...)
{
    if (!handler || level > trace_level::trace) { return; }
    va_list args;
    va_start(args, format);
    emitf(trace_level::trace, format, args);
    va_end(args);
}

void trace::debugf(const char* format, ...)
{
    if (!handler || level > trace_level::debug) { return; }
    va_list args;
    va_start(args, format);
    emitf(trace_level::debug, format, args);
    va_end(args);
}

void trace::infof(const char* format, ...)
{
    if (!handler || level > trace_level::info) { return; }
    va_list args;
    va_start(args, format);
    emitf(trace_level::info, format, args);
    va_end(args);
}

void trace::warnf(const char* format, ...)
{
    if (!handler || level > trace_level::warning) { return; }
    va_list args;
    va_start(args, format);
    emitf(trace_level::warning, format, args);
    va_end(args);
}

void trace::errorf(const char* format, ...)
{
    if (!handler || level > trace_level::error) { return; }
    va_list args;
    va_start(args, format);
    emitf(trace_level::error, format, args);
    va_end(args);
}

// EOF log.cpp
