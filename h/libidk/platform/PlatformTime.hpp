#pragma once

#include "IPlatformFeature.hpp"


namespace idk
{
    class PlatformTime: public IPlatformFeature
    {
    private:

    public:
        PlatformTime(): IPlatformFeature("PlatformTime") {  }
        virtual void update(PlatformContext&) final {  }

        static uint64_t GetSysTimeMs();
        static uint64_t GetSysTimeNs();

    };
}

