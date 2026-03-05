#pragma once

#include <cstddef>
#include <format>

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

