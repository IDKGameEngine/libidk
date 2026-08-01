#include "libidk/platform/PlatformSDL3GL.hpp"
#include "libidk/platform/WindowSDL3.hpp"
#include "libidk/log.hpp"

#include <SDL3/SDL.h>
#include <glad/glad.h>
#include <filesystem>


static void PlatformSDL3GLRaiiFunc()
{
    namespace fs = std::filesystem;

    SDL_SetAppMetadata("BitchAss", "v0.0.0", "com.mellic03.BitchAss");
    fs::current_path(fs::path(SDL_GetBasePath()) / fs::path(IDK_ASSETS_DIRNAME));
    VLOG_INFO("fs::current_path() == {}", fs::current_path().string());

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


idk::platform::PlatformSDL3GL::PlatformSDL3GL(bool headless)
:   mRaii(PlatformSDL3GLRaiiFunc),
    mWin("Window Title", 1280, 720)
{
    (void)headless;
    // SDL_WindowFlags flags = SDL_WINDOW_OPENGL;
    // if (headless) { flags |= SDL_WINDOW_HIDDEN; }
    mWin.makeCurrent();
}


idk::platform::PlatformSDL3GL::~PlatformSDL3GL()
{
    SDL_Quit();
}


void idk::platform::PlatformSDL3GL::update(idk::IEngine *E)
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_EVENT_QUIT)
        {
            E->shutdown();
        }

        if (e.type == SDL_EVENT_KEY_UP)
        {
            if (e.key.scancode == SDL_SCANCODE_ESCAPE)
            {
                E->shutdown();
            }
        }
    }

    mWin.makeCurrent();
}


void idk::platform::PlatformSDL3GL::shutdown(idk::IEngine*)
{

}


idk::platform::Window *idk::platform::PlatformSDL3GL::getWindow()
{
    return &mWin;
}
