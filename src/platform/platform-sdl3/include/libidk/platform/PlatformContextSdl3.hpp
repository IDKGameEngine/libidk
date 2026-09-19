#pragma once

#include "libidk/platform/IPlatformContext.hpp"


namespace idk
{
    struct PlatformConfig
    {
        const char* windowTitle;
        int windowWidth;
        int windowHeight;
    };

    class PlatformContextSdl3: public idk::IPlatformContext
    {
    public:
        PlatformContextSdl3(const PlatformConfig &cfg);
    };

}
