#include "libidk/platform/Platform.hpp"
#include "libidk/log.hpp"

#include <SDL3/SDL.h>
#include <glad/glad.h>
#include <filesystem>

uint64_t idk::platform::getSysTimeMs()
{
    return SDL_GetTicks();
}

uint64_t idk::platform::getSysTimeNs()
{
    return SDL_GetTicksNS();
}


static void PlatformRaiiFunc()
{
    namespace fs = std::filesystem;

    SDL_SetAppMetadata("BitchAss", "v0.0.0", "com.mellic03.BitchAss");
    fs::current_path(fs::path(SDL_GetBasePath()) / fs::path(IDK_ASSETS_DIRNAME));

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


idk::Platform::Platform()
:   mRaii(PlatformRaiiFunc),
    mWin("Window Title", 1280, 720),
    mStat{Status::Running},
    mNumEventFuncs(0)
{
    // SDL_WindowFlags flags = SDL_WINDOW_OPENGL;
    // if (headless) { flags |= SDL_WINDOW_HIDDEN; }
    mWin.makeCurrent();
}


idk::Platform::~Platform()
{
    SDL_Quit();
}


bool idk::Platform::running()
{
    return (mStat.load() == Status::Running);
}


void idk::Platform::shutdown()
{
    mStat.store(Status::ShutdownComplete);
}


void idk::Platform::update()
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        for (size_t i=0; i<mNumEventFuncs; i++)
        {
            mEventFuncs[i](*this, &e, mEventArgs[i]);
        }

        if (e.type == SDL_EVENT_QUIT)
        {
            shutdown();
        }

        if (e.type == SDL_EVENT_KEY_UP)
        {
            if (e.key.scancode == SDL_SCANCODE_ESCAPE)
            {
                shutdown();
            }
        }
    }

    mWin.makeCurrent();
}


idk::platform::Status idk::Platform::getStatus()
{
    return mStat.load();
}


idk::platform::Window &idk::Platform::getWindow()
{
    return mWin;
}


bool idk::Platform::addEventCallback(EventFunc func, void *arg)
{
    if (mNumEventFuncs >= MAX_EVENT_FUNCS)
    {
        VLOG_WARN("[Platform::addEventCallback] mEventFuncs full!");
        return false;
    }

    size_t idx = mNumEventFuncs++;
    mEventFuncs[idx] = func;
    mEventArgs[idx] = arg;

    return true;
}

