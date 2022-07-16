#pragma once

#include <time.h>
#include <stdarg.h>
#include <stdio.h>
#include <cstdlib>
#include <string.h>
#include <string>
using std::string;

namespace yazi {
namespace utility {

#define __DEBUG__
#define __INFO__
#define __WARN__
#define __ERROR__
#define __FATAL__

#ifdef __DEBUG__
#define debug(format, ...) \
    Logger::instance()->log_debug(__FILE__, __LINE__, format, ##__VA_ARGS__)
#else
#define debug(format, ...)
#endif

#ifdef __INFO__
#define info(format, ...) \
    Logger::instance()->log_info(__FILE__, __LINE__, format, ##__VA_ARGS__)
#else
#define info(format, ...)
#endif

#ifdef __WARN__
#define warn(format, ...) \
    Logger::instance()->log_warn(__FILE__, __LINE__, format, ##__VA_ARGS__)
#else
#define warn(format, ...)
#endif

#ifdef __ERROR__
#define error(format, ...) \
    Logger::instance()->log_error(__FILE__, __LINE__, format, ##__VA_ARGS__)
#else
#define error(format, ...)
#endif

#ifdef __FATAL__
#define fatal(format, ...) \
    Logger::instance()->log_fatal(__FILE__, __LINE__, format, ##__VA_ARGS__)
#else
#define fatal(format, ...)
#endif


class Logger
{
public:
    enum Level
    {
        DEBUG = 0,
        INFO,
        WARN,
        ERROR,
        FATAL,
        LEVEL_COUNT
    };

    Logger();
    ~Logger();

    static Logger* instance();
    void open(const string &logfile);
    void close();

    void log_debug(const char* file, int line, const char* format, ...);
    void log_info(const char* file, int line, const char* format, ...);
    void log_warn(const char* file, int line, const char* format, ...);
    void log_error(const char* file, int line, const char* format, ...);
    void log_fatal(const char* file, int line, const char* format, ...);

protected:
    void log(Level level, const char* file, int line, const char* format, va_list arg_ptr);

protected:
    FILE *m_fp;
    static const char* s_level[LEVEL_COUNT];
    static Logger *m_instance;
};

}}
