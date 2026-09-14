#include "Sdl3ContextFactory.hpp"

#include "PlatformContext.hpp"
#include "EventBackend.hpp"
#include "FilesystemBackend.hpp"
#include "InputBackend.hpp"
#include "TimeBackend.hpp"
#include "VideoBackend.hpp"

std::unique_ptr<idk::IPlatformContext>
idk::platform::createSdlPlatform(const PlatformConfig& config)
{
    auto platform = std::make_unique<idk::PlatformContext>();

    platform->addFeature<idk::EventBackend>();
    platform->addFeature<idk::TimeBackend>();
    platform->addFeature<idk::FilesystemBackend>();
    platform->addFeature<idk::InputBackend>();
    platform->addFeature<idk::VideoBackend>(
        config.windowTitle,
        config.windowWidth,
        config.windowHeight
    );

    return platform;
}