#include "libidk/dsa/PeriodicTimer.hpp"
#include "libidk/Assert.hpp"
#include "libidk/math.hpp"
#include "libidk/metric.hpp"
#include "libidk/log.hpp"


idk::PeriodicTimer::PeriodicTimer(uint64_t rateHz)
:   periodNs_(0), startTime_(std::chrono::steady_clock::now())
{
    setRateHz(rateHz);
}


bool idk::PeriodicTimer::expired()
{
    const auto elapsed = std::chrono::steady_clock::now() - startTime_;
    if (elapsed >= std::chrono::nanoseconds(periodNs_))
    {
        return true;
    }
    return false;
}


void idk::PeriodicTimer::reset()
{
    startTime_ = std::chrono::steady_clock::now();
}


void idk::PeriodicTimer::setRateHz(uint64_t rateHz)
{
    IDK_ASSERT(rateHz > 0, "PeriodicTimer rate must be greater than zero");
    periodNs_ = 1000000000 / rateHz;
}
