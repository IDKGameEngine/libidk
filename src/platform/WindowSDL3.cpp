#include "libidk/platform/WindowSDL3.hpp"
#include "libidk/log.hpp"

#include <SDL3/SDL.h>


idk::platform::WindowSDL3::WindowSDL3(const char *title, int w, int h)
:   mTitle(title),
    mWin(SDL_CreateWindow(title, w, h, 0)),
    mWinSurf(SDL_GetWindowSurface(mWin)),
    mBackSurf(SDL_CreateSurface(w, h, SDL_PIXELFORMAT_RGB24)),
    mWidth(mBackSurf->w),
    mHeight(mBackSurf->h),
    mBPP(SDL_GetPixelFormatDetails(mBackSurf->format)->bytes_per_pixel)
{
    if (mWin == NULL)
    {
        VLOG_FATAL("SDL_CreateWindow: {}", SDL_GetError());
    }
}


idk::platform::WindowSDL3::~WindowSDL3()
{
    SDL_DestroyWindow(mWin);
    SDL_Quit();
}


void idk::platform::WindowSDL3::makeCurrent()
{

}


void idk::platform::WindowSDL3::swapWindow()
{
    SDL_Rect srcRect = {
        .x = 0,
        .y = 0,
        .w = mBackSurf->w,
        .h = mBackSurf->h
    };

    SDL_Rect dstRect = {
        .x = 0,
        .y = 0,
        .w = mWinSurf->w,
        .h = mWinSurf->h
    };

    SDL_BlitSurfaceScaled(mBackSurf, &srcRect, mWinSurf, &dstRect, SDL_SCALEMODE_NEAREST);
    SDL_UpdateWindowSurface(mWin);
}


void idk::platform::WindowSDL3::showWindow(bool show)
{
    if (show) { SDL_HideWindow(mWin); }
    else      { SDL_ShowWindow(mWin); }
}


void idk::platform::WindowSDL3::setWindowResolution(int w, int h)
{
    SDL_SetWindowSize(mWin, w, h);
}


void idk::platform::WindowSDL3::setRenderResolution(int w, int h)
{
    SDL_DestroySurface(mBackSurf);
    mBackSurf = SDL_CreateSurface(w, h, SDL_PIXELFORMAT_RGB24);
    mWidth = w;
    mHeight = h;
    mBPP = SDL_GetPixelFormatDetails(mBackSurf->format)->bytes_per_pixel;
}

