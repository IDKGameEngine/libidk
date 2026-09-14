#pragma once

#include "libidk/platform/Platform.hpp"

#include <memory>

struct VkInstance_T;
struct VkSurfaceKHR_T;

namespace idk::sdl3
{
    // Opaque Vulkan handles keep the generic platform API free from Vulkan
    // headers.  The SDL adapter alone knows how to create an SDL-backed surface.
    using VkInstance = ::VkInstance_T *;
    using VkSurfaceKHR = ::VkSurfaceKHR_T *;

    class VulkanSurfaceProvider
    {
    public:
        virtual ~VulkanSurfaceProvider() = default;

        virtual std::span<const char *const> requiredVulkanExtensions() const = 0;
        virtual bool createVulkanSurface(VkInstance instance, VkSurfaceKHR &surface) const = 0;
    };

    // SDL is selected at the composition root.  Its headers and native types do
    // not cross this boundary.
    std::unique_ptr<platform::Platform> makePlatform(platform::AppConfig config = {});

    // This is intentionally an SDL-specific bridge. Renderers that do not use
    // Vulkan never need to include or depend on it.
    VulkanSurfaceProvider *vulkanSurface(platform::Window &window) noexcept;
}
