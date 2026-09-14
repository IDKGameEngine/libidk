#pragma once

#include "libidk/Types.hpp"
#include <chrono>


namespace idk
{
    class PeriodicTimer
    {
    private:
        uint64_t periodNs_;
        std::chrono::steady_clock::time_point startTime_;

    public:
        explicit PeriodicTimer(uint64_t rateHz = 1);
        bool expired();
        void reset();
        void setRateHz(uint64_t rateHz);

        template <typename T> T getPeriodNs() { return static_cast<T>(periodNs_); }
        template <typename T> T getPeriodUs() { return static_cast<T>(periodNs_) / T(1000); }
        template <typename T> T getPeriodMs() { return getPeriodUs<T>() / T(1000); }
        template <typename T> T getPeriodSec() { return getPeriodMs<T>() / T(1000); }
    };
}
