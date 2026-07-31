#pragma once

#include "libidk/platform/WindowSDL3.hpp"

namespace idk::platform
{
    class Platform: public idk::Immobile
    {
    public:
        virtual ~Platform() = default ;
        virtual void update(idk::IEngine*) = 0;
        virtual void shutdown(idk::IEngine*) = 0;
    };
}

