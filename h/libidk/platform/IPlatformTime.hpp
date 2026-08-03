#pragma once

#include "PlatformContext.hpp"
#include "libidk/dsa/PeriodicTimer.hpp"

namespace idk
{
    class IPlatformTime: public IPlatformFeature
    {
    private:

    public:
        virtual uint64_t getSysTimeMs() = 0;
        virtual uint64_t getSysTimeNs() = 0;

        PeriodicTimer createTimer(uint64_t rateHz)
        {
            return PeriodicTimer(this, rateHz);
        }

    };
}

