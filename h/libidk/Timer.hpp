#pragma once

#include "libidk/Platform.hpp"
#include "libidk/math.hpp"
#include "libidk/metric.hpp"
#include "libidk/log.hpp"

namespace idk
{
    class IEngine;


    class PeriodicTimer
    {
    private:
        uint64_t periodNs_;
        uint64_t startTimeNs_;

    public:
        PeriodicTimer(uint64_t rateHz = 1000000000)
        :   periodNs_(0),
            startTimeNs_(Platform::getSysTimeNs())
        {
            setRateHz(rateHz);
        }

        bool expired()
        {
            uint64_t currTimeNs = Platform::getSysTimeNs();
            if ((currTimeNs - startTimeNs_) >= periodNs_)
            {
                return true;
            }
            return false;
        }

        void reset()
        {
            startTimeNs_ = Platform::getSysTimeNs();
        }
    
        void setRateHz(uint64_t rateHz)
        {
            periodNs_ = 1000000000 / rateHz;
        }

        float getExpiryAlpha()
        {
            uint64_t currTimeNs = Platform::getSysTimeNs();
            float alpha = float(currTimeNs - startTimeNs_) / float(periodNs_);
            return idk::clamp(alpha, 0.0f, 1.0f);
        }

        template <typename T> T getPeriodNs() { return static_cast<T>(periodNs_); }
        template <typename T> T getPeriodUs() { return static_cast<T>(periodNs_) / T(1000); }
        template <typename T> T getPeriodMs() { return getPeriodUs<T>() / T(1000); }
        template <typename T> T getPeriodSec() { return getPeriodMs<T>() / T(1000); }
    };
}

// bool idk::MSecTimer::expired()
// {
//     uint64_t curr, delta;

//     curr = SDL_GetTicks();
//     delta = (curr - prev_);

//     if (delta >= step_msec_)
//     {
//         prev_ = curr;
//         return true;
//     }

//     return false;
// }
