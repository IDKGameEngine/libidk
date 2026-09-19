#include "libidk/platform-sdl3/PlatformContext.hpp"
#include "libidk/platform-sdl3/AudioBackend.hpp"
#include "libidk/platform-sdl3/EventBackend.hpp"
#include "libidk/platform-sdl3/FilesystemBackend.hpp"
#include "libidk/platform-sdl3/InputBackend.hpp"
#include "libidk/platform-sdl3/TimeBackend.hpp"
#include "libidk/platform-sdl3/VideoBackend.hpp"

#include <steam/steam_api.h>


idk::PlatformContextSdl3::PlatformContextSdl3(const PlatformConfig &cfg)
{
    if (std::getenv("SteamEnv"))
    {
        SteamErrMsg errMsg = { 0 };
        if (SteamAPI_InitEx(&errMsg) == k_ESteamAPIInitResult_OK)
        {
            VLOG_FATAL("SteamAPI init success");
        }
        else
        {
            VLOG_FATAL("SteamAPI init failure: {}", errMsg);
        }
    }

    addFeature<idk::AudioBackend>();
    addFeature<idk::EventBackend>();
    addFeature<idk::TimeBackend>();
    addFeature<idk::FilesystemBackend>();
    addFeature<idk::InputBackend>();
    addFeature<idk::VideoBackend>(cfg.windowTitle, cfg.windowWidth, cfg.windowHeight);
}

idk::PlatformContextSdl3::~PlatformContextSdl3()
{
    SDL_Quit();
}
