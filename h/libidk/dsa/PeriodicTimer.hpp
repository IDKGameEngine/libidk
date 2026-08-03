#pragma once

#include "libidk/platform/IPlatformTime.hpp"
#include "libidk/math.hpp"
#include "libidk/metric.hpp"
#include "libidk/log.hpp"


namespace idk
{
    class PeriodicTimer
    {
    private:
        friend class IPlatformTime;
        IPlatformTime *mTime;
        uint64_t periodNs_;
        uint64_t startTimeNs_;

        PeriodicTimer(IPlatformTime *p, uint64_t rateHz = 1000000000)
        :   mTime(p), periodNs_(0), startTimeNs_(mTime->getSysTimeNs())
        {
            setRateHz(rateHz);
        }

    public:
        bool expired()
        {
            uint64_t currTimeNs = mTime->getSysTimeNs();
            if ((currTimeNs - startTimeNs_) >= periodNs_)
            {
                return true;
            }
            return false;
        }

        void reset()
        {
            startTimeNs_ = mTime->getSysTimeNs();
        }
    
        void setRateHz(uint64_t rateHz)
        {
            periodNs_ = 1000000000 / rateHz;
        }

        float getExpiryAlpha()
        {
            uint64_t currTimeNs = mTime->getSysTimeNs();
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
