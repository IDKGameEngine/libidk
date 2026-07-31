#pragma once

#include "libidk/platform/WindowSDL3GL.hpp"
#include "libidk/raii.hpp"
#include "idk_engine/Engine.hpp"
#include "libidk/metric.hpp"

struct SDL_Window;

namespace idk::platform
{
    class PlatformSDL3GL: public idk::Immobile
    {
    public:
        PlatformSDL3GL(bool headless = false);
        ~PlatformSDL3GL();
        void update(idk::IEngine*);
        void shutdown(idk::IEngine*);

    private:
        RaiiFunc<void()>            mRaii;
        idk::platform::WindowSDL3GL mWin;

    };
}

