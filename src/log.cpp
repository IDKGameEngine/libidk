#include "libidk/Assert.hpp"
#include "libidk/log.hpp"
#include "ansi.hpp"

#include <cstdarg>
#include <cstdio>
#include <mutex>


extern "C"
{
    static void *vl_fh_ = nullptr;

    __attribute__((constructor))
    static void vl_init(void)
    {
        vl_fh_ = stdout;
    }
}


void idk::vlog(idk::LogType type, const char *title, const char *fmt, ...)
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


    // static std::mutex logmutex;
    {
        // std::lock_guard<std::mutex> lock(logmutex);
    
    #ifdef IDKLOG_VERBOSE
        fprintf((std::FILE*)vl_fh_, "%s[%s]%s[%s] ", color, severity, ANSI::RESET, title);
    #else
        (void)title;
        fprintf((std::FILE*)vl_fh_, "%s[%s]%s ", color, severity, ANSI::RESET);
    #endif

        va_list vlist;
        va_start(vlist, fmt);
        std::vfprintf((std::FILE*)vl_fh_, fmt, vlist);
        va_end(vlist);

        fprintf((std::FILE*)vl_fh_, "\n");

        if (type == LogType::FATAL)
        {
            fflush((std::FILE*)vl_fh_);
            IDK_ASSERT(false, "Exiting");
        }
    }
}





using namespace idk;

int SysLog::indent_ = 0;

SysLog::SysLog(std::FILE *fh, const char *fmt, ...)
:   fh_(fh)
{
    SysLog::_putIndent(fh_);
    SysLog::_print(fh_, "[");

    va_list vlist;
    va_start(vlist, fmt);
    vfprintf(fh_, fmt, vlist);
    va_end(vlist);

    SysLog::_print(fh_, "]\n");
    SysLog::_putIndent(fh_);
    SysLog::_print(fh_, "{\n");
    _pushIndent();
}

SysLog::~SysLog()
{
    _popIndent();
    SysLog::_putIndent(fh_);
    SysLog::_print(fh_, "}\n");
}

void SysLog::operator()(const char *fmt, ...)
{
    SysLog::_putIndent(fh_);

    va_list vlist;
    va_start(vlist, fmt);
    vfprintf(fh_, fmt, vlist);
    va_end(vlist);

    fprintf(fh_, "\n");
}


void SysLog::_pushIndent(int n) { indent_ += n; }
void SysLog::_popIndent (int n) { indent_ -= n; }
void SysLog::_putIndent(std::FILE *fh)
{
    for (int i=0; i<indent_; i++)
    {
        fprintf(fh, " ");
    }
}

void SysLog::_print(std::FILE *fh, const char *fmt, ... )
{
    va_list vlist;
    va_start(vlist, fmt);
    vfprintf(fh, fmt, vlist);
    va_end(vlist);
}
