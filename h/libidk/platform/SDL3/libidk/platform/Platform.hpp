#pragma once

#include "Window.hpp"
#include "libidk/raii.hpp"
#include "libidk/Engine.hpp"

namespace idk::platform
{
    class Platform: public idk::platform::IPlatform
    {
    private:
        RaiiFunc<void()>      mRaii;
        idk::platform::Window mWin;

    public:
        Platform(bool headless = false);
        ~Platform();
        virtual void update(idk::IEngine*) final;
        virtual void shutdown(idk::IEngine*) final;
        virtual Window *getWindow() final { return &mWin; };

    };

}

