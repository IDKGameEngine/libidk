#pragma once

#include "libidk/Types.hpp"
#include "libidk/Assert.hpp"
#include "libidk/metric.hpp"
#include <chrono>


namespace idk
{
    class PeriodicTimer
    {
    private:
        uint64_t periodNs_;
        std::chrono::steady_clock::time_point startTime_;

    public:
        PeriodicTimer(uint64_t rateHz = 1)
        :   periodNs_(0),
            startTime_(std::chrono::steady_clock::now())
        {
            setRateHz(rateHz);
        }

        bool expired()
        {
            const auto elapsed = std::chrono::steady_clock::now() - startTime_;
            if (elapsed >= std::chrono::nanoseconds(periodNs_))
            {
                return true;
            }
            return false;
        }
        
        void reset()
        {
            startTime_ = std::chrono::steady_clock::now();
        }

        void setRateHz(uint64_t rateHz)
        {
            IDK_ASSERT(rateHz > 0, "PeriodicTimer rate must be greater than zero");
            periodNs_ = 1000000000 / rateHz;
        }

        template <typename T> T getPeriodNs() { return static_cast<T>(periodNs_); }
        template <typename T> T getPeriodUs() { return static_cast<T>(periodNs_) / T(1000); }
        template <typename T> T getPeriodMs() { return getPeriodUs<T>() / T(1000); }
        template <typename T> T getPeriodSec() { return getPeriodMs<T>() / T(1000); }
    };


    class FixedAccumulator
    {
    private:
        double mStepSec;
        double mAccumSec;

    public:
        FixedAccumulator(double rateHz = 60.0)
        :   mStepSec(0.0), mAccumSec(0.0)
        {
            setRateHz(rateHz);
        }

        void setRateHz(double rateHz)
        {
            IDK_ASSERT(rateHz > 0.0, "rateHz must be greater than zero");
            mStepSec = 1.0 / rateHz;
        }

        void addDeltaTimeSec(double deltaTime)
        {
            mAccumSec += deltaTime;
        }

        bool step()
        {
            if (mAccumSec < mStepSec)
            {
                return false;
            }
            mAccumSec -= mStepSec;
            return true;
        }

        double getStepSec() const
        {
            return mStepSec;
        }
    };

}
