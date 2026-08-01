#pragma once

#include "libidk/platform/Platform.hpp"
#include "libidk/platform/WindowSDL3.hpp"
#include "libidk/raii.hpp"
#include "libidk/Engine.hpp"

namespace idk::platform
{
    class PlatformSDL3: public idk::platform::Platform
    {
    public:
        PlatformSDL3(bool headless = false);
        ~PlatformSDL3();
        virtual void update(idk::IEngine*) final;
        virtual void shutdown(idk::IEngine*) final;
        virtual Window *getWindow() final;

    private:
        RaiiFunc<void()>          mRaii;
        idk::platform::WindowSDL3 mWin;
    };

}

