#include "libidk/platform/PlatformContextSdl3.hpp"

#include "libidk/platform/IPlatformContext.hpp"
#include "EventBackend.hpp"
#include "FilesystemBackend.hpp"
#include "InputBackend.hpp"
#include "TimeBackend.hpp"
#include "VideoBackend.hpp"

idk::PlatformContextSdl3::PlatformContextSdl3(const PlatformConfig &cfg)
{
    addFeature<idk::EventBackend>();
    addFeature<idk::TimeBackend>();
    addFeature<idk::FilesystemBackend>();
    addFeature<idk::InputBackend>();
    addFeature<idk::VideoBackend>(cfg.windowTitle, cfg.windowWidth, cfg.windowHeight);
}

