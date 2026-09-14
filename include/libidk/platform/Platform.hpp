#pragma once

#include <chrono>
#include <cstdint>
#include <filesystem>
#include <span>
#include <string>

namespace idk::platform
{
    struct Extent
    {
        int width = 0;
        int height = 0;
    };

    enum class EventType
    {
        quit,
        key_down,
        key_up,
        window_resized,
    };

    // Platform events are engine-owned values.  Backends must translate their
    // native event structures before exposing them to the rest of the program.
    struct Event
    {
        EventType type;
        int key = 0;
        Extent extent {};
    };

    struct AppConfig
    {
        std::string title = "IDK";
        Extent initialWindowSize { 1280, 720 };
        std::filesystem::path assetDirectory = "data";
    };

    class Window
    {
    public:
        virtual ~Window() = default;

        virtual Extent extent() const = 0;
        virtual void setExtent(Extent extent) = 0;
        virtual void setVisible(bool visible) = 0;
    };

    class Platform
    {
    public:
        virtual ~Platform() = default;

        // Starts a new event frame. events() remains valid until the next call.
        virtual void pollEvents() = 0;
        virtual std::span<const Event> events() const = 0;
        virtual bool quitRequested() const = 0;

        virtual Window &mainWindow() = 0;
        virtual std::chrono::nanoseconds monotonicTime() const = 0;
        virtual const std::filesystem::path &assetDirectory() const = 0;
    };
}
