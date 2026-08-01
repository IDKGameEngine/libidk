#pragma once

#include "libidk/platform/Window.hpp"

struct SDL_Window;

namespace idk::platform
{
    class WindowSDL3GL: public idk::platform::Window
    {
    private:
        const char   *mTitle;
        SDL_Window   *mWin;
        void         *mGlCtx;
        int32_t       mWidth;
        int32_t       mHeight;

    public:
        WindowSDL3GL(const char *title, int w, int h);
        ~WindowSDL3GL();

        virtual int   getWidth() const final { return mWidth; };
        virtual int   getHeight() const final { return mHeight; };
        virtual void *getNativeHandle() const final { return mWin; };
        virtual void *getGpuContext() const final { return mGlCtx; };
        virtual void  makeCurrent() final;
        virtual void  swapWindow() final;
        virtual void  showWindow(bool show) final;
        virtual void  setWindowResolution(int w, int h) final;
        virtual void  setRenderResolution(int w, int h) final;

    };
}
