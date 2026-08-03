#include "libidk/dsa/PeriodicTimer.hpp"
#include "libidk/platform/IPlatformTime.hpp"
#include "libidk/math.hpp"
#include "libidk/metric.hpp"
#include "libidk/log.hpp"


idk::PeriodicTimer::PeriodicTimer(IPlatformTime *p, uint64_t rateHz = 1000000000)
:   mTime(p), periodNs_(0), startTimeNs_(p->getSysTimeNs())
{
    setRateHz(rateHz);
}


bool idk::PeriodicTimer::expired()
{
    uint64_t currTimeNs = mTime->getSysTimeNs();
    if ((currTimeNs - startTimeNs_) >= periodNs_)
    {
        return true;
    }
    return false;
}


void idk::PeriodicTimer::reset()
{
    startTimeNs_ = mTime->getSysTimeNs();
}


void idk::PeriodicTimer::setRateHz(uint64_t rateHz)
{
    periodNs_ = 1000000000 / rateHz;
}

