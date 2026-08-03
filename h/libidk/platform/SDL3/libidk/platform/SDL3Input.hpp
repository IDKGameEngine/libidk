#pragma once

#include "libidk/platform/IPlatformInput.hpp"


namespace idk
{
    class SDL3Input: public IPlatformInput
    {
    private:

    public:
        SDL3Input();
        virtual void update(PlatformContext&) final;

    };

}

