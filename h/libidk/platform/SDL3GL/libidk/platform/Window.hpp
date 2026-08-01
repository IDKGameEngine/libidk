#pragma once

#include "libidk/platform/IPlatform.hpp"

struct SDL_Window;

namespace idk::platform
{
    class Window: public idk::Immobile
    {
    private:
        const char   *mTitle;
        SDL_Window   *mWin;
        void         *mGlCtx;
        int32_t       mWidth;
        int32_t       mHeight;

    public:
        Window(const char *title, int w, int h);
        ~Window();

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
