#pragma once

#include <stdint.h>
#include <stddef.h>

namespace idk
{
    static constexpr size_t KILO = 1024;
    static constexpr size_t MEGA = 1024 * idk::KILO;
    static constexpr size_t GIGA = 1024 * idk::MEGA;

    namespace time
    {
        static constexpr uint64_t msec_to_usec(uint64_t msec)
        {
            return 1000*msec;
        }

        static constexpr uint64_t msec_to_nsec(uint64_t msec)
        {
            return 1000*1000*msec;
        }
        static constexpr uint64_t msec_to_nsec(double msec)
        {
            return static_cast<uint64_t>(1000.0 * 1000.0 * msec);
        }

        static constexpr uint64_t usec_to_msec(uint64_t usec)
        {
            return usec/1000;
        }

        static constexpr uint64_t usec_to_nsec(uint64_t usec)
        {
            return 1000*usec;
        }

        static constexpr uint64_t nsec_to_msec(uint64_t nsec)
        {
            return nsec/(1000*1000);
        }

        static constexpr uint64_t nsec_to_usec(uint64_t nsec)
        {
            return nsec/1000;
        }
    }
}

