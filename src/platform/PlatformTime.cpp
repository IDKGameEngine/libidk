#pragma once

#include "IPlatformFeature.hpp"


namespace idk
{
    class PlatformTime: public IPlatformFeature
    {
    private:

    public:
        PlatformTime(): IPlatformFeature("PlatformTime") {  }

        static uint64_t GetSysTimeMs();
        static uint64_t GetSysTimeNs();

    };
}

