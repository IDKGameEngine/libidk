#pragma once

#include "IPlatformFeature.hpp"


namespace idk
{
    class PlatformFileSystem: public IPlatformFeature
    {
    private:
        
    public:
        PlatformFileSystem(): IPlatformFeature("PlatformFileSystem") {  }
        virtual void update(PlatformContext&) final;

    };
}
