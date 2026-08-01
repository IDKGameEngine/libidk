#pragma once

#include "libidk/platform/Window.hpp"

namespace idk::platform
{
    uint64_t getSysTimeMs();
    uint64_t getSysTimeNs();

    class Platform: public idk::Immobile
    {
    public:
        virtual ~Platform() = default ;
        virtual void update(idk::IEngine*) = 0;
        virtual void shutdown(idk::IEngine*) = 0;
        virtual Window *getWindow() { return nullptr; }
    };

}

