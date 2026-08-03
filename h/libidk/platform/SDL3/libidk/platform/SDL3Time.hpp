#pragma once

#include "libidk/platform/IPlatformTime.hpp"


namespace idk
{
    class SDL3Time: public IPlatformTime
    {
    private:

    public:
        SDL3Time();

        virtual uint64_t getSysTimeMs() final;
        virtual uint64_t getSysTimeNs() final;

    };
}

