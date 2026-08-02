#include "libidk/platform/Platform.hpp"
#include "libidk/log.hpp"

#include <SDL3/SDL.h>
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

    // SDL_SetAppMetadata("BitchAss", "v0.0.0", "com.mellic03.BitchAss");
    fs::current_path(fs::path(SDL_GetBasePath()) / fs::path(IDK_ASSETS_DIRNAME));
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO))
    {
        VLOG_FATAL("{}", SDL_GetError());
    }
}


idk::platform::Platform::Platform()
:   mRaii(PlatformRaiiFunc),
    mWin("Window Title", 1280, 720),
    mStat{Status::Running}
{

}


idk::platform::Platform::~Platform()
{

}



void idk::platform::Platform::update()
{

}

void idk::platform::Platform::shutdown()
{

}

idk::platform::Platform::Status idk::platform::Platform::getStatus()
{

}


void idk::platform::Platform::update()
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
}
