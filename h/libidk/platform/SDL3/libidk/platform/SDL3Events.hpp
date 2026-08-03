#pragma once

#include "libidk/platform/IPlatformEvents.hpp"


namespace idk
{
    class SDL3Events: public IPlatformEvents
    {
    private:

    public:
        SDL3Events();
        virtual void update(PlatformContext&) final;

    };
}

