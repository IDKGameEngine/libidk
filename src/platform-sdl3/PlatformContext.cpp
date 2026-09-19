#include "libidk/platform-sdl3/PlatformContext.hpp"
#include "libidk/platform-sdl3/AudioBackend.hpp"
#include "libidk/platform-sdl3/EventBackend.hpp"
#include "libidk/platform-sdl3/FilesystemBackend.hpp"
#include "libidk/platform-sdl3/InputBackend.hpp"
#include "libidk/platform-sdl3/TimeBackend.hpp"
#include "libidk/platform-sdl3/VideoBackend.hpp"

idk::PlatformContextSdl3::PlatformContextSdl3(const PlatformConfig &cfg)
{
    addFeature<idk::AudioBackend>();
    addFeature<idk::EventBackend>();
    addFeature<idk::TimeBackend>();
    addFeature<idk::FilesystemBackend>();
    addFeature<idk::InputBackend>();
    addFeature<idk::VideoBackend>(cfg.windowTitle, cfg.windowWidth, cfg.windowHeight);
}

