// Copyright (c) 2014-2017 Jae-jun Kang
// See the file LICENSE for details.

#ifndef X2BOOST_TRACE_HPP_
#define X2BOOST_TRACE_HPP_

#ifndef X2BOOST_PRE_HPP_
#include "x2boost/pre.hpp"
#endif

#include <ostream>
#include <sstream>

#include <boost/thread/tss.hpp>

namespace x2boost
{
    /// Specifies the trace log level.
    class X2BOOST_API trace_level
    {
    public:
        static const int all = 0;
        static const int trace = 1;
        static const int debug = 2;
        static const int info = 3;
        static const int warning = 4;
        static const int error = 5;
        static const int none = 6;
    
        static std::string string(int value);
    };

    /// Represents a trace log stream.
    class X2BOOST_API trace_stream : public std::ostream
    {
    public:
        trace_stream(int level)
            : std::ostream(&buf_), buf_(this), level_(level) {}
    private:
        class buf : public std::stringbuf
        {
        public:
            buf(trace_stream* stream) : stream_(stream) {}
        protected:
            virtual int sync();
            trace_stream* stream_;
        };
        buf buf_;
        int level_;
    };

    /// Represents the trace logging helper class.
    class X2BOOST_API trace
    {
    public:
        /// Trace log handler function type.
        typedef void (*handler_func)(int, const std::string&);
        
        static void emit(int trace_level, const std::string& message)
        {
            if (!handler || trace_level < level) { return; }
            (*handler)(trace_level, message);
        }

        static void log(const std::string& message)
        {
            emit(trace_level::trace, message);
        }
        static void logf(const char* format, ...);

        static void debug(const std::string& message)
        {
            emit(trace_level::debug, message);
        }
        static void debugf(const char* format, ...);

        static void info(const std::string& message)
        {
            emit(trace_level::info, message);
        }
        static void infof(const char* format, ...);

        static void warn(const std::string& message)
        {
            emit(trace_level::warning, message);
        }
        static void warnf(const char* format, ...);

        static void error(const std::string& message)
        {
            emit(trace_level::error, message);
        }
        static void errorf(const char* format, ...);

        static std::ostream& log()
        {
            if (!trace_.get()) { trace_.reset(new trace_stream(trace_level::trace)); }
            return *trace_;
        }
        static std::ostream& debug()
        {
            if (!debug_.get()) { debug_.reset(new trace_stream(trace_level::debug)); }
            return *debug_;
        }
        static std::ostream& info()
        {
            if (!info_.get()) { info_.reset(new trace_stream(trace_level::info)); }
            return *info_;
        }
        static std::ostream& warning()
        {
            if (!warning_.get()) { warning_.reset(new trace_stream(trace_level::warning)); }
            return *warning_;
        }
        static std::ostream& error()
        {
            if (!error_.get()) { error_.reset(new trace_stream(trace_level::error)); }
            return *error_;
        }

        static int level;
        static handler_func handler;
    
    private:
        static boost::thread_specific_ptr<trace_stream> trace_;
        static boost::thread_specific_ptr<trace_stream> debug_;
        static boost::thread_specific_ptr<trace_stream> info_;
        static boost::thread_specific_ptr<trace_stream> warning_;
        static boost::thread_specific_ptr<trace_stream> error_;
    };
}

#endif  // X2BOOST_TRACE_HPP_
