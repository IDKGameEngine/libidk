#pragma once

#include "libidk/Types.hpp"

#include <cstdarg>
#include <cstdio>
#include <format>

// namespace idk::core
// {
//     static constexpr size_t MAX_LOG_STRING_LENGTH = 1024;

//     void logEvent(const char *fmt, ...);
// }


namespace idk
{
    enum class LogType: int
    {
        INFO, WARN, ERROR, FATAL
    };

    void vnlog(idk::LogType type, const char *title, const char *fmt, ...);
}

#define VLOG_INFO(...) \
    idk::vnlog(idk::LogType::INFO, __PRETTY_FUNCTION__, std::format(__VA_ARGS__).c_str())

#define VLOG_WARN(...) \
    idk::vnlog(idk::LogType::WARN, __PRETTY_FUNCTION__, std::format(__VA_ARGS__).c_str())

#define VLOG_ERROR(...) \
    idk::vnlog(idk::LogType::ERROR, __PRETTY_FUNCTION__, std::format(__VA_ARGS__).c_str())

#define VLOG_FATAL(...) \
    idk::vnlog(idk::LogType::FATAL, __PRETTY_FUNCTION__, std::format(__VA_ARGS__).c_str())



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
