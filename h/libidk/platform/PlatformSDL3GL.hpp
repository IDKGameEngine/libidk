#pragma once

#include "libidk/platform/WindowSDL3GL.hpp"
#include "libidk/raii.hpp"
#include "libidk/Engine.hpp"

namespace idk::platform
{
    class PlatformSDL3GL: public idk::Immobile
    {
    public:
        PlatformSDL3GL(bool headless = false);
        ~PlatformSDL3GL();
        virtual void update(idk::IEngine*) final;
        virtual void shutdown(idk::IEngine*) final;

    private:
        RaiiFunc<void()>            mRaii;
        idk::platform::WindowSDL3GL mWin;

    };
}

