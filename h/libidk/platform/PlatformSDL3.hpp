#pragma once

#include "libidk/platform/WindowSDL3.hpp"
#include "libidk/raii.hpp"
#include "libidk/Engine.hpp"
#include "libidk/metric.hpp"

struct SDL_Window;

namespace idk::platform
{
    class PlatformSDL3: public idk::Immobile
    {
    public:
        PlatformSDL3(bool headless = false);
        ~PlatformSDL3();
        void update(idk::IEngine*);
        void shutdown(idk::IEngine*);

    private:
        RaiiFunc<void()>          mRaii;
        idk::platform::WindowSDL3 mWin;

    };
}

