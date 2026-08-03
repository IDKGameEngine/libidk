#include "libidk/platform/IPlatformVideo.hpp"
#include "libidk/log.hpp"

#include <glad/glad.h>
#include <GL/glu.h>
#include <SDL3/SDL.h>


static void PlatformVideoRaiiFunc()
{
    if (false == SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO))
    {
        VLOG_FATAL("{}", SDL_GetError());
    }

    if (!SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE))
    {
        VLOG_ERROR("{}", SDL_GetError());
    }

    if (!SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4))
    {
        VLOG_ERROR("{}", SDL_GetError());
    }

    if (!SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6))
    {
        VLOG_ERROR("{}", SDL_GetError());
    }

    if (!SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4))
    {
        VLOG_ERROR("{}", SDL_GetError());
    }

    if (!SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1))
    {
        VLOG_ERROR("{}", SDL_GetError());
    }

    if (!SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,  24))
    {
        VLOG_ERROR("{}", SDL_GetError());
    }

    if (!SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8))
    {
        VLOG_ERROR("{}", SDL_GetError());
    }
}


idk::PlatformVideo::PlatformVideo(const char *title, int w, int h)
:   mTitle(title),
    mWin(nullptr),
    mWidth(w),
    mHeight(h)
{
    VLOG_INFO("[Window::Window]");

    if (!(mWin = SDL_CreateWindow(title, mWidth, mHeight, SDL_WINDOW_OPENGL)))
    {
        VLOG_FATAL("SDL_CreateWindow: {}", SDL_GetError());
    }

    if (!(mGlCtx = SDL_GL_CreateContext(mWin)))
    {
        VLOG_FATAL("SDL_GL_CreateContext: {}", SDL_GetError());
    }

    if (!SDL_GL_SetSwapInterval(1))
    {
        VLOG_WARN("SDL_GL_SetSwapInterval: {}", SDL_GetError());
    }

    int interval;
    if (!SDL_GL_GetSwapInterval(&interval))
    {
        VLOG_WARN("SDL_GL_GetSwapInterval: {}", SDL_GetError());
    }
    else
    {
        VLOG_INFO("SDL_GL_GetSwapInterval: interval={}", interval);
    }

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        VLOG_FATAL("gladLoadGLLoader failure");
    }

}


idk::platform::Window::~Window()
{
    SDL_GL_DestroyContext((SDL_GLContext)mGlCtx);
    SDL_DestroyWindow(mWin);
    SDL_Quit();
}


void idk::platform::Window::makeCurrent()
{
    SDL_GL_MakeCurrent(mWin, (SDL_GLContext)mGlCtx);
}


void idk::platform::Window::swapWindow()
{
    SDL_GL_SwapWindow(mWin);
}


void idk::platform::Window::showWindow(bool show)
{
    if (show) { SDL_ShowWindow(mWin); }
    else      { SDL_HideWindow(mWin); }
}


void idk::platform::Window::setWindowResolution(int w, int h)
{
    SDL_SetWindowSize(mWin, w, h);
}


void idk::platform::Window::setRenderResolution(int w, int h)
{
    (void)w;
    (void)h;
}
