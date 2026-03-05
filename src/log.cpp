#include "libidk/log.hpp"
#include "ansi.hpp"
#include <cstdarg>
#include <cstdio>

using namespace idk;

extern "C"
{
    static void *vl_outfile_ = nullptr;

    __attribute__((constructor))
    static void vl_init(void)
    {
        vl_outfile_ = stdout;
    }
}


void idk::vnlog(idk::LogType type, const char *title, const char *fmt, ...)
{
    const char *severity = "should_not_happen";
    const char *color = ANSI::RESET;

    switch (type)
    {
        using enum idk::LogType;

        case INFO:
            severity = "INFO";
            color = ANSI::Reg::GRN;
            break;
        case WARN:
            severity = "WARN";
            color = ANSI::Reg::YEL;
            break;
        case ERROR:
            severity = "ERROR";
            color = ANSI::Reg::RED;
            break;
        case FATAL:
            severity = "FATAL";
            color = ANSI::BldHi::RED;
            break;
        default:
            break;
    }

#ifdef IDKLOG_VERBOSE
    fprintf((std::FILE*)vl_outfile_, "%s[%s]%s[%s] ", color, severity, ANSI::RESET, title);
#else
    (void)title;
    fprintf((std::FILE*)vl_outfile_, "%s[%s]%s ", color, severity, ANSI::RESET);
#endif

    va_list vlist;
    va_start(vlist, fmt);
    std::vfprintf((std::FILE*)vl_outfile_, fmt, vlist);
    va_end(vlist);

    fprintf((std::FILE*)vl_outfile_, "\n");

    if (type == LogType::FATAL)
    {
        exit(1);
    }
}

