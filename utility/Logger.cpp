#include "Logger.h"

using namespace yazi::utility;

const char* Logger::s_level[LEVEL_COUNT] = 
{
    "DEBUG",
    "INFO",
    "WARN",
    "ERROR",
    "FATAL"
};

Logger *Logger::m_instance = NULL;

Logger::Logger() : m_fp(NULL)
{
}

Logger::~Logger()
{
    close();
}

Logger* Logger::instance()
{
    if (m_instance == NULL)
        m_instance = new Logger();
    return m_instance;
}

void Logger::open(const string &logfile)
{
    close();
    m_fp = fopen(logfile.c_str(), "a+");
    if (m_fp == NULL)
    {
        printf("open log file failed: %s\n", logfile.c_str());
        exit(1);
    }
}

void Logger::close()
{
    if (m_fp != NULL)
    {
        fclose(m_fp);
        m_fp = NULL;
    }
}

void Logger::log_debug(const char* file, int line, const char* format, ...)
{
    va_list arg_ptr;
    va_start(arg_ptr, format);
    log(DEBUG, file, line, format, arg_ptr);
    va_end(arg_ptr);
}

void Logger::log_info(const char* file, int line, const char* format, ...)
{
    va_list arg_ptr;
    va_start(arg_ptr, format);
    log(INFO, file, line, format, arg_ptr);
    va_end(arg_ptr);
}

void Logger::log_warn(const char* file, int line, const char* format, ...)
{
    va_list arg_ptr;
    va_start(arg_ptr, format);
    log(WARN, file, line, format, arg_ptr);
    va_end(arg_ptr);
}

void Logger::log_error(const char* file, int line, const char* format, ...)
{
    va_list arg_ptr;
    va_start(arg_ptr, format);
    log(ERROR, file, line, format, arg_ptr);
    va_end(arg_ptr);
}

void Logger::log_fatal(const char* file, int line, const char* format, ...)
{
    va_list arg_ptr;
    va_start(arg_ptr, format);
    log(FATAL, file, line, format, arg_ptr);
    va_end(arg_ptr);
}

void Logger::log(Level level, const char* file, int line, const char* format, va_list arg_ptr)
{
    if (m_fp == NULL)
    {
        printf("open log file failed: m_fp == NULL\n");
        exit(1);
    }
    time_t ticks = time(NULL);
    struct tm* ptm = localtime(&ticks);
    char buf[32];
    memset(buf, 0, sizeof(buf));
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S  ", ptm);
    flockfile(m_fp);
    fprintf(m_fp, buf);
    fprintf(m_fp, "%s  ", s_level[level]);
    fprintf(m_fp, "%s:%d  ", file, line);
    vfprintf(m_fp, format, arg_ptr);
    fprintf(m_fp, "\r\n");
    fflush(m_fp);
    funlockfile(m_fp);
}


