#include "libidk/platform-sdl3/SdlPlatform.hpp"

#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>

#include <cstdlib>
#include <memory>
#include <utility>
#include <vector>

namespace idk::sdl3
{
    namespace
    {
        class SdlWindow final: public platform::Window, public VulkanSurfaceProvider
        {
        public:
            SdlWindow(const std::string &title, platform::Extent extent)
            {
                window_ = SDL_CreateWindow(title.c_str(), extent.width, extent.height, SDL_WINDOW_VULKAN);
                if (window_ == nullptr)
                {
                    SDL_Log("SDL_CreateWindow failed: %s", SDL_GetError());
                    std::abort();
                }
            }

            ~SdlWindow() override
            {
                SDL_DestroyWindow(window_);
            }

            platform::Extent extent() const override
            {
                int width = 0;
                int height = 0;
                SDL_GetWindowSize(window_, &width, &height);
                return { width, height };
            }

            void setExtent(platform::Extent extent) override
            {
                SDL_SetWindowSize(window_, extent.width, extent.height);
            }

            void setVisible(bool visible) override
            {
                if (visible)
                {
                    SDL_ShowWindow(window_);
                }
                else
                {
                    SDL_HideWindow(window_);
                }
            }

            std::span<const char *const> requiredVulkanExtensions() const override
            {
                uint32_t count = 0;
                const char *const *extensions = SDL_Vulkan_GetInstanceExtensions(&count);
                if (extensions == nullptr)
                {
                    return {};
                }

                vulkanExtensions_.assign(extensions, extensions + count);
                return vulkanExtensions_;
            }

            bool createVulkanSurface(VkInstance instance, VkSurfaceKHR &surface) const override
            {
                return SDL_Vulkan_CreateSurface(window_, instance, nullptr, &surface);
            }

        private:
            SDL_Window *window_ = nullptr;
            mutable std::vector<const char *> vulkanExtensions_;
        };

        class SdlPlatform final: public platform::Platform
        {
        public:
            explicit SdlPlatform(platform::AppConfig config)
            {
                if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS))
                {
                    SDL_Log("SDL_Init failed: %s", SDL_GetError());
                    std::abort();
                }

                const char *basePath = SDL_GetBasePath();
                if (basePath != nullptr)
                {
                    assetDirectory_ = std::filesystem::path(basePath) / config.assetDirectory;
                }
                else
                {
                    assetDirectory_ = std::move(config.assetDirectory);
                }

                window_ = std::make_unique<SdlWindow>(config.title, config.initialWindowSize);
            }

            ~SdlPlatform() override
            {
                window_.reset();
                SDL_Quit();
            }

            void pollEvents() override
            {
                events_.clear();

                SDL_Event nativeEvent;
                while (SDL_PollEvent(&nativeEvent))
                {
                    switch (nativeEvent.type)
                    {
                        case SDL_EVENT_QUIT:
                            quitRequested_ = true;
                            events_.push_back({ platform::EventType::quit });
                            break;
                        case SDL_EVENT_KEY_DOWN:
                            events_.push_back({ platform::EventType::key_down, static_cast<int>(nativeEvent.key.scancode) });
                            break;
                        case SDL_EVENT_KEY_UP:
                            events_.push_back({ platform::EventType::key_up, static_cast<int>(nativeEvent.key.scancode) });
                            break;
                        case SDL_EVENT_WINDOW_RESIZED:
                            events_.push_back({
                                platform::EventType::window_resized,
                                0,
                                { nativeEvent.window.data1, nativeEvent.window.data2 },
                            });
                            break;
                        default:
                            break;
                    }
                }
            }

            std::span<const platform::Event> events() const override
            {
                return events_;
            }

            bool quitRequested() const override
            {
                return quitRequested_;
            }

            platform::Window &mainWindow() override
            {
                return *window_;
            }

            std::chrono::nanoseconds monotonicTime() const override
            {
                return std::chrono::nanoseconds(SDL_GetTicksNS());
            }

            const std::filesystem::path &assetDirectory() const override
            {
                return assetDirectory_;
            }

        private:
            std::unique_ptr<SdlWindow> window_;
            std::vector<platform::Event> events_;
            std::filesystem::path assetDirectory_;
            bool quitRequested_ = false;
        };
    }

    std::unique_ptr<platform::Platform> makePlatform(platform::AppConfig config)
    {
        return std::make_unique<SdlPlatform>(std::move(config));
    }

    VulkanSurfaceProvider *vulkanSurface(platform::Window &window) noexcept
    {
        return dynamic_cast<VulkanSurfaceProvider *>(&window);
    }
}
