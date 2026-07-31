#pragma once

#include <cstdint>


namespace idk
{
    /**
     * Must be implemented application-side
     */
    class Platform
    {
    public:
        static uint64_t getSysTimeMs();
        static uint64_t getSysTimeNs();
    };
}
