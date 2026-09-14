#pragma once

#include "libidk/platform/IPlatformContext.hpp"

#include <memory>

namespace idk::platform
{
    struct PlatformConfig
    {
        const char* windowTitle;
        int windowWidth;
        int windowHeight;
    };

    std::unique_ptr<IPlatformContext> createSdlPlatform(const PlatformConfig& config);
}