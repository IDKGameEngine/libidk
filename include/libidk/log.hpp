#pragma once

#include "libidk/Types.hpp"

#include <cstdarg>
#include <cstdio>
#include <format>


namespace idk
{
    enum class LogType: int
    {
        INFO, WARN, ERROR, FATAL
    };

    void vlog(idk::LogType type, const char *title, const char *fmt, ...);
}

#define VLOG_INFO(...) idk::vlog(idk::LogType::INFO, __PRETTY_FUNCTION__, std::format(__VA_ARGS__).c_str())
#define VLOG_WARN(...) idk::vlog(idk::LogType::WARN, __PRETTY_FUNCTION__, std::format(__VA_ARGS__).c_str())
#define VLOG_ERROR(...) idk::vlog(idk::LogType::ERROR, __PRETTY_FUNCTION__, std::format(__VA_ARGS__).c_str())
#define VLOG_FATAL(...) idk::vlog(idk::LogType::FATAL, __PRETTY_FUNCTION__, std::format(__VA_ARGS__).c_str())

namespace idk
{
    class SysLog
    {
    public:
        template <typename... Args>
        SysLog(const char *fmt, Args ...args): SysLog(stdout, fmt, args...) {  };
        SysLog(std::FILE *dst, const char *fmt, ...);
        ~SysLog();
        void operator()(const char*, ...);

    private:
        std::FILE *fh_;
        
        static int32_t indent_; 
        static void _pushIndent(int n=4);
        static void _popIndent(int n=4);
        static void _putIndent(std::FILE*);
        static void _print(std::FILE*, const char *fmt, ...);
    };
}
