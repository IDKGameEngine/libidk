#pragma once

#include "libidk/platform/Window.hpp"

struct SDL_Window;

namespace idk::platform
{
    class WindowSDL3GL: public idk::platform::Window
    {
    public:
        WindowSDL3GL(const char *title, int w, int h);
        ~WindowSDL3GL();

        virtual int   getWidth() const final;
        virtual int   getHeight() const final;
        virtual void *getNativeHandle() const final;
        virtual void *getGpuContext() const final;
        virtual void  pollEvents() final;
        virtual void  makeCurrent() final;
        virtual void  swapWindow() final;
        virtual void  setWindowResolution(int w, int h) final;
        virtual void  setRenderResolution(int w, int h) final;

        const char   *mTitle;
        SDL_Window   *mWin;
        void         *mGlCtx;
        int32_t       mWidth;
        int32_t       mHeight;
    };
}
