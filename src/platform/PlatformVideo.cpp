#pragma once

#include "IPlatformFeature.hpp"


namespace idk
{
    class PlatformVideo: public IPlatformFeature
    {
    private:

    public:
        PlatformVideo(): IPlatformFeature("PlatformVideo") {  }

        virtual int getWidth() const = 0;
        virtual int getHeight() const = 0;

    };
}
