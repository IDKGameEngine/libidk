#pragma once

#include "IPlatformFeature.hpp"
#include "libidk/dsa/PeriodicTimer.hpp"


namespace idk
{
    class ITimeBackend: public idk::IPlatformFeature
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

