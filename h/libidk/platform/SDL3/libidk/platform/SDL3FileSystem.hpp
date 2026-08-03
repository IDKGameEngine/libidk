#pragma once

#include "libidk/platform/PlatformFileSystem.hpp"


namespace idk
{
    class SDL3FileSystem: public PlatformFileSystem
    {
    private:
        
    public:
        SDL3FileSystem();
        virtual void update(PlatformContext&) final;

    };
}
