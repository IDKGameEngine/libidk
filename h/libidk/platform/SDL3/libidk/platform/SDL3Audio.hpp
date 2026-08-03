#pragma once

#include "libidk/platform/IPlatformAudio.hpp"


namespace idk
{
    class SDL3Audio: public IPlatformAudio
    {
    private:
        
    public:
        SDL3Audio();
        virtual void update(PlatformContext&) final;

    };
}
