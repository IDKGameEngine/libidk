#pragma once

#include "IPlatformFeature.hpp"


namespace idk
{
    class PlatformEvents: public IPlatformFeature
    {
    private:

    public:
        PlatformEvents(): IPlatformFeature("PlatformEvents") {  }

    };
}

