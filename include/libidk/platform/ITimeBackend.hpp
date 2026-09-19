#pragma once

#include "IPlatformFeature.hpp"


namespace idk
{
    class ITimeBackend: public idk::IPlatformFeature
    {
    private:

    public:
        virtual uint64_t getSysTimeMs() = 0;
        virtual uint64_t getSysTimeNs() = 0;

    };
}

