#pragma once

#include "libidk/Types.hpp"
#include "libidk/platform/IPlatformContext.hpp"


namespace idk
{
    class PeriodicTimer
    {
    private:
        friend class ITimeBackend;
        ITimeBackend *mTime;
        uint64_t periodNs_;
        uint64_t startTimeNs_;
        PeriodicTimer(ITimeBackend*, uint64_t rateHz);

    public:
        bool expired();
        void reset();
        void setRateHz(uint64_t rateHz);

        template <typename T> T getPeriodNs() { return static_cast<T>(periodNs_); }
        template <typename T> T getPeriodUs() { return static_cast<T>(periodNs_) / T(1000); }
        template <typename T> T getPeriodMs() { return getPeriodUs<T>() / T(1000); }
        template <typename T> T getPeriodSec() { return getPeriodMs<T>() / T(1000); }
    };
}

