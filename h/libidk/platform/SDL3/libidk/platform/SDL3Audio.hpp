#pragma once

#include "IPlatformFeature.hpp"


namespace idk
{
    class PlatformAudio: public IPlatformFeature
    {
    private:
        
    public:
        PlatformAudio(): IPlatformFeature("PlatformAudio") {  }
        virtual void update(PlatformContext&) final;

    };
}
