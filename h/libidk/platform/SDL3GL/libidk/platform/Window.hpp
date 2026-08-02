#pragma once

#include "libidk/Types.hpp"
struct SDL_Window;

namespace idk::platform
{
    using WinCtxType = SDL_Window;
    using GpuCtxType = void;

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

        WinCtxType *getWinCtx() const { return mWin; };
        GpuCtxType *getGpuCtx() const { return mGlCtx; };

        int getWidth() const { return mWidth; };
        int getHeight() const { return mHeight; };

        void makeCurrent();
        void swapWindow();
        void showWindow(bool show);
        void setWindowResolution(int w, int h);
        void setRenderResolution(int w, int h);

    };
}
