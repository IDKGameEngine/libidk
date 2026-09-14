#pragma once

#include "libidk/platform/IPlatformContext.hpp"


namespace idk
{
    class PlatformContext: public idk::IPlatformContext
    {
    public:
        PlatformContext(): IPlatformContext() {  };
    };
}
