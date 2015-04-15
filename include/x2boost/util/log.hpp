// Copyright (c) 2014-2015 Jae-jun Kang
// See the file LICENSE for details.

#ifndef X2BOOST_LOG_HPP_
#define X2BOOST_LOG_HPP_

#ifndef X2BOOST_PRE_HPP_
#include "x2boost/pre.hpp"
#endif

#include <ostream>
#include <sstream>

#include <boost/thread/tss.hpp>

namespace x2
{
    class X2BOOST_API log_level
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

    class X2BOOST_API log_stream : public std::ostream
    {
    public:
        log_stream(int level)
            : std::ostream(&buf_), buf_(this), level_(level) {}
    private:
        class buf : public std::stringbuf
        {
        public:
            buf(log_stream* stream) : stream_(stream) {}
        protected:
            virtual int sync();
            log_stream* stream_;
        };
        buf buf_;
        int level_;
    };

    class X2BOOST_API log
    {
    public:
        typedef void (*handler_func)(int, const std::string&);
        
        static void emit(int log_level, const std::string& message)
        {
            if (!handler || log_level < level) { return; }
            (*handler)(log_level, message);
        }

        static void trace(const std::string& message)
        {
            emit(log_level::trace, message);
        }
        static void tracef(const char* format, ...);

        static void debug(const std::string& message)
        {
            emit(log_level::debug, message);
        }
        static void debugf(const char* format, ...);

        static void info(const std::string& message)
        {
            emit(log_level::info, message);
        }
        static void infof(const char* format, ...);

        static void warn(const std::string& message)
        {
            emit(log_level::warning, message);
        }
        static void warnf(const char* format, ...);

        static void error(const std::string& message)
        {
            emit(log_level::error, message);
        }
        static void errorf(const char* format, ...);

        static std::ostream& trace()
        {
            if (!trace_.get()) { trace_.reset(new log_stream(log_level::trace)); }
            return *trace_;
        }
        static std::ostream& debug()
        {
            if (!debug_.get()) { debug_.reset(new log_stream(log_level::debug)); }
            return *debug_;
        }
        static std::ostream& info()
        {
            if (!info_.get()) { info_.reset(new log_stream(log_level::info)); }
            return *info_;
        }
        static std::ostream& warning()
        {
            if (!warning_.get()) { warning_.reset(new log_stream(log_level::warning)); }
            return *warning_;
        }
        static std::ostream& error()
        {
            if (!error_.get()) { error_.reset(new log_stream(log_level::error)); }
            return *error_;
        }

        static int level;
        static handler_func handler;
    
    private:
        static boost::thread_specific_ptr<log_stream> trace_;
        static boost::thread_specific_ptr<log_stream> debug_;
        static boost::thread_specific_ptr<log_stream> info_;
        static boost::thread_specific_ptr<log_stream> warning_;
        static boost::thread_specific_ptr<log_stream> error_;
    };
}

#endif  // X2BOOST_LOG_HPP_
