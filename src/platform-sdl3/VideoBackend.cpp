#include "libidk/platform-sdl3/VideoBackend.hpp"
#include "libidk/platform-sdl3/PlatformContext.hpp"
#include "libidk/Assert.hpp"
#include "libidk/log.hpp"

#define VK_NO_PROTOTYPES
#include <volk/volk.h>
 
// #define VMA_IMPLEMENTATION
// #include <vk_mem_alloc.h>
 
#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>

#include <algorithm>
#include <vector>


idk::VideoBackend::VideoBackend(const char *title, int w, int h)
:   mTitle(title),
    mWin(nullptr),
    mInstance(VK_NULL_HANDLE),
    mWidth(w),
    mHeight(h)
{
    if (false == SDL_Init(SDL_INIT_VIDEO))
    {
        VLOG_FATAL("{}", SDL_GetError());
    }

    if (!(mWin = SDL_CreateWindow(title, mWidth, mHeight, SDL_WINDOW_VULKAN)))
    {
        VLOG_FATAL("SDL_CreateWindow: {}", SDL_GetError());
    }

    volkInitialize();

    // uint32_t instanceExtensionsCount { 0 };
    // auto *instanceExtensions = SDL_Vulkan_GetInstanceExtensions(&instanceExtensionsCount);
    // if (instanceExtensions == nullptr)
    // {
    //     VLOG_FATAL("SDL_Vulkan_GetInstanceExtensions: {}", SDL_GetError());
    // }

    // VkApplicationInfo appInfo {};
    // appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    // appInfo.pApplicationName = title;
    // appInfo.applicationVersion = VK_MAKE_API_VERSION(0, 0, 1, 0);
    // appInfo.apiVersion = VK_API_VERSION_1_4;

    // std::vector<const char*> enabledExtensions;
    // enabledExtensions.assign(instanceExtensions, instanceExtensions + instanceExtensionsCount);

    // VkInstanceCreateInfo instanceCI {};
    // instanceCI.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    // instanceCI.pApplicationInfo = &appInfo;
    // instanceCI.enabledExtensionCount = static_cast<uint32_t>(enabledExtensions.size());
    // instanceCI.ppEnabledExtensionNames = enabledExtensions.data();

    // IDK_ASSERT(VK_SUCCESS == vkCreateInstance(&instanceCI, nullptr, &mInstance), "vkCreateInstance failed");
    // volkLoadInstance(mInstance);

    // VkSurfaceKHR surface = VK_NULL_HANDLE;
    // if (!SDL_Vulkan_CreateSurface((SDL_Window*)mWin, mInstance, nullptr, &surface))
    // {
    //     VLOG_FATAL("SDL_Vulkan_CreateSurface: {}", SDL_GetError());
    // }
}


idk::VideoBackend::~VideoBackend()
{
    // if (mInstance != VK_NULL_HANDLE)
    // {
    //     vkDestroyInstance(mInstance, nullptr);
    //     mInstance = VK_NULL_HANDLE;
    // }

    if (mWin != nullptr)
    {
        SDL_DestroyWindow((SDL_Window*)mWin);
        mWin = nullptr;
    }
}


void idk::VideoBackend::update(idk::IPlatformContext *ctx)
{
    (void)ctx;
}


void idk::VideoBackend::setWindowVisibility(bool visible)
{
    if (mWin == nullptr)
    {
        return;
    }

    if (visible)
    {
        SDL_ShowWindow((SDL_Window*)mWin);
    }
    else
    {
        SDL_HideWindow((SDL_Window*)mWin);
    }
}

void idk::VideoBackend::setWindowResolution(int w, int h)
{
    if (mWin == nullptr)
    {
        mWidth = w;
        mHeight = h;
        return;
    }

    SDL_SetWindowSize((SDL_Window*)mWin, w, h);
    mWidth = w;
    mHeight = h;
}

void idk::VideoBackend::setRenderResolution(int w, int h)
{
    mWidth = w;
    mHeight = h;

    if (mWin != nullptr)
    {
        SDL_SetWindowSize((SDL_Window*)mWin, w, h);
    }
}
