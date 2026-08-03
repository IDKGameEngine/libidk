#pragma once

#include "libidk/platform/IPlatformVideo.hpp"
#include "Platform.hpp"

struct SDL_Window;


class idk::PlatformVideo: public idk::IPlatformVideo
{
private:
    const char *mTitle;
    SDL_Window *mWin;
    void       *mGlCtx;
    int32_t     mWidth;
    int32_t     mHeight;

public:
    PlatformVideo(const char *title, int w, int h);
    
    virtual int getWidth()  const final { return mWidth; }
    virtual int getHeight() const final { return mHeight; }

};



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
