#pragma once

#include "IPlatformFeature.hpp"


namespace idk
{
    class PlatformInput: public IPlatformFeature
    {
    private:

    public:
        PlatformInput(): IPlatformFeature("PlatformInput") {  }
        virtual void update(PlatformContext&) final;

    };

}

