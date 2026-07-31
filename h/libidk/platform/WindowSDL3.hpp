#pragma once

#include "libidk/platform/Window.hpp"

struct SDL_Window;
struct SDL_Surface;

namespace idk::platform
{
    class WindowSDL3: public idk::platform::Window
    {
    public:
        WindowSDL3(const char *title, int w, int h);
        ~WindowSDL3();

        virtual int   getWidth() const final;
        virtual int   getHeight() const final;
        virtual void *getNativeHandle() const final;
        virtual void  pollEvents() final;
        virtual void  makeCurrent() final;
        virtual void  swapWindow() final;
        virtual void  showWindow(bool) final;
        virtual void  setWindowResolution(int w, int h) final;
        virtual void  setRenderResolution(int w, int h) final;

    private:
        const char   *mTitle;
        SDL_Window   *mWin;
        SDL_Surface  *mWinSurf;
        SDL_Surface  *mBackSurf;
        int32_t       mWidth;
        int32_t       mHeight;
        int32_t       mPitch;
        int32_t       mBPP;

    };
}
