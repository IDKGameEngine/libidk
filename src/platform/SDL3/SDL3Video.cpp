#include "libidk/platform/SDL3Video.hpp"
#include "libidk/log.hpp"

#define VOLK_IMPLEMENTATION
#include <volk.h>
 
#define VMA_IMPLEMENTATION
#include <vk_mem_alloc.h>
 
#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
 

idk::SDL3Video::SDL3Video(const char *title, int w, int h)
:   mTitle(title),
    mWin(nullptr),
    mGl(nullptr),
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

    VkInstance instance;
    VkApplicationInfo appInfo;
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "How to Vulkan";
    appInfo.apiVersion = VK_API_VERSION_1_4;

    uint32_t instanceExtensionsCount { 0 };
    char const* const* instanceExtensions { SDL_Vulkan_GetInstanceExtensions(&instanceExtensionsCount) };

    VkInstanceCreateInfo instanceCI;
    instanceCI.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceCI.pApplicationInfo = &appInfo;
    instanceCI.enabledExtensionCount = instanceExtensionsCount;
    instanceCI.ppEnabledExtensionNames = instanceExtensions;

    IDK_ASSERT(VK_SUCCESS == vkCreateInstance(&instanceCI, nullptr, &instance), "Ruh roh");


    // if (!SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE))
    // {
    //     VLOG_ERROR("{}", SDL_GetError());
    // }

    // if (!SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4))
    // {
    //     VLOG_ERROR("{}", SDL_GetError());
    // }

    // if (!SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6))
    // {
    //     VLOG_ERROR("{}", SDL_GetError());
    // }

    // if (!SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4))
    // {
    //     VLOG_ERROR("{}", SDL_GetError());
    // }

    // if (!SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1))
    // {
    //     VLOG_ERROR("{}", SDL_GetError());
    // }

    // if (!SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,  24))
    // {
    //     VLOG_ERROR("{}", SDL_GetError());
    // }

    // if (!SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8))
    // {
    //     VLOG_ERROR("{}", SDL_GetError());
    // }

    // if (!(mGl = SDL_GL_CreateContext((SDL_Window*)mWin)))
    // {
    //     VLOG_FATAL("SDL_GL_CreateContext: {}", SDL_GetError());
    // }

    // if (!SDL_GL_SetSwapInterval(1))
    // {
    //     VLOG_WARN("SDL_GL_SetSwapInterval: {}", SDL_GetError());
    // }

    // int interval;
    // if (!SDL_GL_GetSwapInterval(&interval))
    // {
    //     VLOG_WARN("SDL_GL_GetSwapInterval: {}", SDL_GetError());
    // }
    // else
    // {
    //     VLOG_INFO("SDL_GL_GetSwapInterval: interval={}", interval);
    // }

    // if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    // {
    //     VLOG_FATAL("gladLoadGLLoader failure");
    // }
}


idk::SDL3Video::~SDL3Video()
{
    SDL_GL_DestroyContext((SDL_GLContext)mGl);
    SDL_DestroyWindow((SDL_Window*)mWin);
    SDL_Quit();
}


void idk::SDL3Video::update(PlatformContext &ctx)
{
    (void)ctx;
    SDL_GL_SwapWindow((SDL_Window*)mWin);
}


void idk::SDL3Video::setWindowVisibility(bool visible)
{
    if (visible) { SDL_ShowWindow((SDL_Window*)mWin); }
    else         { SDL_HideWindow((SDL_Window*)mWin); }
}


void idk::SDL3Video::setWindowResolution(int w, int h)
{
    SDL_SetWindowSize((SDL_Window*)mWin, w, h);
}


void idk::SDL3Video::setRenderResolution(int w, int h)
{
    (void)w;
    (void)h;
}
