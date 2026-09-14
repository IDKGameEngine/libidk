#include "VideoBackend.hpp"
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

namespace
{
    static constexpr VkFormat kSwapchainFormat = VK_FORMAT_B8G8R8A8_SRGB;
}

void idk::VideoBackend::createLogicalDevice()
{
    uint32_t deviceCount = 0;
    IDK_ASSERT(VK_SUCCESS == vkEnumeratePhysicalDevices(mInstance, &deviceCount, nullptr), "vkEnumeratePhysicalDevices failed");
    if (deviceCount == 0)
    {
        VLOG_FATAL("No Vulkan physical devices found");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    IDK_ASSERT(VK_SUCCESS == vkEnumeratePhysicalDevices(mInstance, &deviceCount, devices.data()), "vkEnumeratePhysicalDevices failed");

    mPhysicalDevice = VK_NULL_HANDLE;
    for (VkPhysicalDevice candidate : devices)
    {
        VkPhysicalDeviceProperties props {};
        vkGetPhysicalDeviceProperties(candidate, &props);
        (void)props;
        uint32_t queueCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(candidate, &queueCount, nullptr);
        std::vector<VkQueueFamilyProperties> queues(queueCount);
        vkGetPhysicalDeviceQueueFamilyProperties(candidate, &queueCount, queues.data());

        for (uint32_t i = 0; i < queueCount; ++i)
        {
            VkBool32 supportsPresent = VK_FALSE;
            vkGetPhysicalDeviceSurfaceSupportKHR(candidate, i, mSurface, &supportsPresent);
            if (queues[i].queueFlags & VK_QUEUE_GRAPHICS_BIT && supportsPresent)
            {
                mPhysicalDevice = candidate;
                break;
            }
        }

        if (mPhysicalDevice != VK_NULL_HANDLE)
        {
            break;
        }
    }

    IDK_ASSERT(mPhysicalDevice != VK_NULL_HANDLE, "No Vulkan device with graphics+present support found");

    uint32_t queueFamilyIndex = 0;
    uint32_t queueCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(mPhysicalDevice, &queueCount, nullptr);
    std::vector<VkQueueFamilyProperties> queues(queueCount);
    vkGetPhysicalDeviceQueueFamilyProperties(mPhysicalDevice, &queueCount, queues.data());

    for (uint32_t i = 0; i < queueCount; ++i)
    {
        VkBool32 supportsPresent = VK_FALSE;
        vkGetPhysicalDeviceSurfaceSupportKHR(mPhysicalDevice, i, mSurface, &supportsPresent);
        if (queues[i].queueFlags & VK_QUEUE_GRAPHICS_BIT && supportsPresent)
        {
            queueFamilyIndex = i;
            break;
        }
    }

    const float queuePriority = 1.0f;
    VkDeviceQueueCreateInfo queueCI {};
    queueCI.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCI.queueFamilyIndex = queueFamilyIndex;
    queueCI.queueCount = 1;
    queueCI.pQueuePriorities = &queuePriority;

    std::vector<const char *> enabledExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
    VkDeviceCreateInfo deviceCI {};
    deviceCI.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCI.queueCreateInfoCount = 1;
    deviceCI.pQueueCreateInfos = &queueCI;
    deviceCI.enabledExtensionCount = static_cast<uint32_t>(enabledExtensions.size());
    deviceCI.ppEnabledExtensionNames = enabledExtensions.data();

    IDK_ASSERT(VK_SUCCESS == vkCreateDevice(mPhysicalDevice, &deviceCI, nullptr, &mDevice), "vkCreateDevice failed");
    volkLoadDevice(mDevice);

    vkGetDeviceQueue(mDevice, queueFamilyIndex, 0, &mGraphicsQueue);
}

void idk::VideoBackend::createSwapchain()
{
    VkSurfaceCapabilitiesKHR caps {};
    IDK_ASSERT(VK_SUCCESS == vkGetPhysicalDeviceSurfaceCapabilitiesKHR(mPhysicalDevice, mSurface, &caps), "vkGetPhysicalDeviceSurfaceCapabilitiesKHR failed");

    uint32_t formatCount = 0;
    IDK_ASSERT(VK_SUCCESS == vkGetPhysicalDeviceSurfaceFormatsKHR(mPhysicalDevice, mSurface, &formatCount, nullptr), "vkGetPhysicalDeviceSurfaceFormatsKHR failed");
    std::vector<VkSurfaceFormatKHR> formats(formatCount);
    IDK_ASSERT(VK_SUCCESS == vkGetPhysicalDeviceSurfaceFormatsKHR(mPhysicalDevice, mSurface, &formatCount, formats.data()), "vkGetPhysicalDeviceSurfaceFormatsKHR failed");

    VkSurfaceFormatKHR surfaceFormat = formats[0];
    for (const auto &fmt : formats)
    {
        if (fmt.format == kSwapchainFormat)
        {
            surfaceFormat = fmt;
            break;
        }
    }
    mSwapchainFormat = surfaceFormat.format;

    VkPresentModeKHR presentMode = VK_PRESENT_MODE_FIFO_KHR;
    uint32_t presentCount = 0;
    IDK_ASSERT(VK_SUCCESS == vkGetPhysicalDeviceSurfacePresentModesKHR(mPhysicalDevice, mSurface, &presentCount, nullptr), "vkGetPhysicalDeviceSurfacePresentModesKHR failed");
    std::vector<VkPresentModeKHR> presentModes(presentCount);
    IDK_ASSERT(VK_SUCCESS == vkGetPhysicalDeviceSurfacePresentModesKHR(mPhysicalDevice, mSurface, &presentCount, presentModes.data()), "vkGetPhysicalDeviceSurfacePresentModesKHR failed");
    for (VkPresentModeKHR mode : presentModes)
    {
        if (mode == VK_PRESENT_MODE_MAILBOX_KHR)
        {
            presentMode = mode;
            break;
        }
    }

    VkExtent2D extent = caps.currentExtent;
    if (extent.width == UINT32_MAX)
    {
        extent.width = static_cast<uint32_t>(mWidth);
        extent.height = static_cast<uint32_t>(mHeight);
        extent.width = std::clamp(extent.width, caps.minImageExtent.width, caps.maxImageExtent.width);
        extent.height = std::clamp(extent.height, caps.minImageExtent.height, caps.maxImageExtent.height);
    }
    mSwapchainExtent = extent;

    uint32_t minImageCount = caps.minImageCount + 1;
    if (caps.maxImageCount > 0 && minImageCount > caps.maxImageCount)
    {
        minImageCount = caps.maxImageCount;
    }

    VkSwapchainCreateInfoKHR swapchainCI {};
    swapchainCI.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchainCI.surface = mSurface;
    swapchainCI.minImageCount = minImageCount;
    swapchainCI.imageFormat = surfaceFormat.format;
    swapchainCI.imageColorSpace = surfaceFormat.colorSpace;
    swapchainCI.imageExtent = extent;
    swapchainCI.imageArrayLayers = 1;
    swapchainCI.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    swapchainCI.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    swapchainCI.preTransform = caps.currentTransform;
    swapchainCI.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapchainCI.presentMode = presentMode;
    swapchainCI.clipped = VK_TRUE;
    swapchainCI.oldSwapchain = VK_NULL_HANDLE;

    IDK_ASSERT(VK_SUCCESS == vkCreateSwapchainKHR(mDevice, &swapchainCI, nullptr, &mSwapchain), "vkCreateSwapchainKHR failed");

    uint32_t imageCount = 0;
    IDK_ASSERT(VK_SUCCESS == vkGetSwapchainImagesKHR(mDevice, mSwapchain, &imageCount, nullptr), "vkGetSwapchainImagesKHR failed");
    mSwapImages.resize(imageCount);
    IDK_ASSERT(VK_SUCCESS == vkGetSwapchainImagesKHR(mDevice, mSwapchain, &imageCount, mSwapImages.data()), "vkGetSwapchainImagesKHR failed");
}

void idk::VideoBackend::createRenderPass()
{
    VkAttachmentDescription colorAttachment {};
    colorAttachment.format = mSwapchainFormat;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentRef {};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass {};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;

    VkSubpassDependency dependency {};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    VkRenderPassCreateInfo renderPassCI {};
    renderPassCI.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassCI.attachmentCount = 1;
    renderPassCI.pAttachments = &colorAttachment;
    renderPassCI.subpassCount = 1;
    renderPassCI.pSubpasses = &subpass;
    renderPassCI.dependencyCount = 1;
    renderPassCI.pDependencies = &dependency;

    IDK_ASSERT(VK_SUCCESS == vkCreateRenderPass(mDevice, &renderPassCI, nullptr, &mRenderPass), "vkCreateRenderPass failed");
}

void idk::VideoBackend::createFramebuffers()
{
    mSwapImageViews.clear();
    mFramebuffers.clear();

    for (VkImage swapImage : mSwapImages)
    {
        VkImageViewCreateInfo viewCI {};
        viewCI.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewCI.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewCI.format = mSwapchainFormat;
        viewCI.image = swapImage;
        viewCI.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        viewCI.subresourceRange.baseMipLevel = 0;
        viewCI.subresourceRange.levelCount = 1;
        viewCI.subresourceRange.baseArrayLayer = 0;
        viewCI.subresourceRange.layerCount = 1;

        VkImageView imageView = VK_NULL_HANDLE;
        IDK_ASSERT(VK_SUCCESS == vkCreateImageView(mDevice, &viewCI, nullptr, &imageView), "vkCreateImageView failed");
        mSwapImageViews.push_back(imageView);

        VkFramebufferCreateInfo framebufferCI {};
        framebufferCI.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferCI.renderPass = mRenderPass;
        framebufferCI.attachmentCount = 1;
        framebufferCI.pAttachments = &mSwapImageViews.back();
        framebufferCI.width = mSwapchainExtent.width;
        framebufferCI.height = mSwapchainExtent.height;
        framebufferCI.layers = 1;

        VkFramebuffer framebuffer = VK_NULL_HANDLE;
        IDK_ASSERT(VK_SUCCESS == vkCreateFramebuffer(mDevice, &framebufferCI, nullptr, &framebuffer), "vkCreateFramebuffer failed");
        mFramebuffers.push_back(framebuffer);
    }
}

void idk::VideoBackend::createCommandBuffer()
{
    VkCommandPoolCreateInfo poolCI {};
    poolCI.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolCI.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolCI.queueFamilyIndex = 0;

    IDK_ASSERT(VK_SUCCESS == vkCreateCommandPool(mDevice, &poolCI, nullptr, &mCommandPool), "vkCreateCommandPool failed");

    VkCommandBufferAllocateInfo allocCI {};
    allocCI.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocCI.commandPool = mCommandPool;
    allocCI.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocCI.commandBufferCount = 1;

    IDK_ASSERT(VK_SUCCESS == vkAllocateCommandBuffers(mDevice, &allocCI, &mCommandBuffer), "vkAllocateCommandBuffers failed");

    VkSemaphoreCreateInfo semCI {};
    semCI.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    IDK_ASSERT(VK_SUCCESS == vkCreateSemaphore(mDevice, &semCI, nullptr, &mImageAvailableSemaphore), "vkCreateSemaphore failed");
    IDK_ASSERT(VK_SUCCESS == vkCreateSemaphore(mDevice, &semCI, nullptr, &mRenderFinishedSemaphore), "vkCreateSemaphore failed");
}

void idk::VideoBackend::recreateSwapchain()
{
    for (VkFramebuffer framebuffer : mFramebuffers)
    {
        if (framebuffer != VK_NULL_HANDLE)
        {
            vkDestroyFramebuffer(mDevice, framebuffer, nullptr);
        }
    }
    mFramebuffers.clear();

    for (VkImageView view : mSwapImageViews)
    {
        if (view != VK_NULL_HANDLE)
        {
            vkDestroyImageView(mDevice, view, nullptr);
        }
    }
    mSwapImageViews.clear();

    if (mRenderPass != VK_NULL_HANDLE)
    {
        vkDestroyRenderPass(mDevice, mRenderPass, nullptr);
        mRenderPass = VK_NULL_HANDLE;
    }

    if (mSwapchain != VK_NULL_HANDLE)
    {
        vkDestroySwapchainKHR(mDevice, mSwapchain, nullptr);
        mSwapchain = VK_NULL_HANDLE;
    }

    createSwapchain();
    createRenderPass();
    createFramebuffers();
}

idk::VideoBackend::VideoBackend(const char *title, int w, int h)
:   mTitle(title),
    mWin(nullptr),
    mInstance(VK_NULL_HANDLE),
    mSurface(VK_NULL_HANDLE),
    mPhysicalDevice(VK_NULL_HANDLE),
    mDevice(VK_NULL_HANDLE),
    mGraphicsQueue(VK_NULL_HANDLE),
    mSwapchain(VK_NULL_HANDLE),
    mRenderPass(VK_NULL_HANDLE),
    mCommandPool(VK_NULL_HANDLE),
    mCommandBuffer(VK_NULL_HANDLE),
    mImageAvailableSemaphore(VK_NULL_HANDLE),
    mRenderFinishedSemaphore(VK_NULL_HANDLE),
    mSwapchainFormat(VK_FORMAT_UNDEFINED),
    mSwapchainExtent{0, 0},
    mImageIndex(0),
    mWidth(w),
    mHeight(h),
    mClearColor{0.1f, 0.2f, 0.3f, 1.0f}
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

    uint32_t instanceExtensionsCount { 0 };
    auto *instanceExtensions = SDL_Vulkan_GetInstanceExtensions(&instanceExtensionsCount);
    if (instanceExtensions == nullptr)
    {
        VLOG_FATAL("SDL_Vulkan_GetInstanceExtensions: {}", SDL_GetError());
    }

    VkApplicationInfo appInfo {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = title;
    appInfo.applicationVersion = VK_MAKE_API_VERSION(0, 0, 1, 0);
    appInfo.apiVersion = VK_API_VERSION_1_4;

    std::vector<const char *> enabledExtensions;
    enabledExtensions.assign(instanceExtensions, instanceExtensions + instanceExtensionsCount);

    VkInstanceCreateInfo instanceCI {};
    instanceCI.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceCI.pApplicationInfo = &appInfo;
    instanceCI.enabledExtensionCount = static_cast<uint32_t>(enabledExtensions.size());
    instanceCI.ppEnabledExtensionNames = enabledExtensions.data();

    IDK_ASSERT(VK_SUCCESS == vkCreateInstance(&instanceCI, nullptr, &mInstance), "vkCreateInstance failed");
    volkLoadInstance(mInstance);

    VkSurfaceKHR surface = VK_NULL_HANDLE;
    if (!SDL_Vulkan_CreateSurface((SDL_Window*)mWin, mInstance, nullptr, &surface))
    {
        VLOG_FATAL("SDL_Vulkan_CreateSurface: {}", SDL_GetError());
    }
    mSurface = surface;

    createLogicalDevice();
    createSwapchain();
    createRenderPass();
    createFramebuffers();
    createCommandBuffer();
}


idk::VideoBackend::~VideoBackend()
{
    if (mDevice != VK_NULL_HANDLE)
    {
        vkDeviceWaitIdle(mDevice);
        if (mRenderFinishedSemaphore != VK_NULL_HANDLE) { vkDestroySemaphore(mDevice, mRenderFinishedSemaphore, nullptr); mRenderFinishedSemaphore = VK_NULL_HANDLE; }
        if (mImageAvailableSemaphore != VK_NULL_HANDLE) { vkDestroySemaphore(mDevice, mImageAvailableSemaphore, nullptr); mImageAvailableSemaphore = VK_NULL_HANDLE; }
        if (mCommandBuffer != VK_NULL_HANDLE) { vkFreeCommandBuffers(mDevice, mCommandPool, 1, &mCommandBuffer); mCommandBuffer = VK_NULL_HANDLE; }
        if (mCommandPool != VK_NULL_HANDLE) { vkDestroyCommandPool(mDevice, mCommandPool, nullptr); mCommandPool = VK_NULL_HANDLE; }
        for (VkFramebuffer framebuffer : mFramebuffers)
        {
            if (framebuffer != VK_NULL_HANDLE)
            {
                vkDestroyFramebuffer(mDevice, framebuffer, nullptr);
            }
        }
        mFramebuffers.clear();
        for (VkImageView view : mSwapImageViews)
        {
            if (view != VK_NULL_HANDLE)
            {
                vkDestroyImageView(mDevice, view, nullptr);
            }
        }
        mSwapImageViews.clear();
        if (mRenderPass != VK_NULL_HANDLE) { vkDestroyRenderPass(mDevice, mRenderPass, nullptr); mRenderPass = VK_NULL_HANDLE; }
        if (mSwapchain != VK_NULL_HANDLE) { vkDestroySwapchainKHR(mDevice, mSwapchain, nullptr); mSwapchain = VK_NULL_HANDLE; }
        vkDestroyDevice(mDevice, nullptr);
        mDevice = VK_NULL_HANDLE;
    }

    if (mSurface != VK_NULL_HANDLE)
    {
        vkDestroySurfaceKHR(mInstance, mSurface, nullptr);
        mSurface = VK_NULL_HANDLE;
    }

    if (mInstance != VK_NULL_HANDLE)
    {
        vkDestroyInstance(mInstance, nullptr);
        mInstance = VK_NULL_HANDLE;
    }

    if (mWin != nullptr)
    {
        SDL_DestroyWindow((SDL_Window*)mWin);
        mWin = nullptr;
    }

    SDL_Quit();
}


void idk::VideoBackend::setClearColor(float r, float g, float b, float a)
{
    mClearColor[0] = r;
    mClearColor[1] = g;
    mClearColor[2] = b;
    mClearColor[3] = a;
}

void idk::VideoBackend::getClearColor(float &r, float &g, float &b, float &a) const
{
    r = mClearColor[0];
    g = mClearColor[1];
    b = mClearColor[2];
    a = mClearColor[3];
}


void idk::VideoBackend::update(idk::IPlatformContext &ctx)
{
    (void)ctx;
    SDL_PumpEvents();

    if (mWin != nullptr)
    {
        int w = 0;
        int h = 0;
        SDL_GetWindowSize((SDL_Window*)mWin, &w, &h);
        mWidth = w;
        mHeight = h;
    }

    if (mDevice == VK_NULL_HANDLE || mRenderPass == VK_NULL_HANDLE || mFramebuffers.empty())
    {
        return;
    }

    uint32_t imageIndex = 0;
    VkResult acquireResult = vkAcquireNextImageKHR(mDevice, mSwapchain, UINT64_MAX, mImageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);
    if (acquireResult == VK_ERROR_OUT_OF_DATE_KHR || acquireResult == VK_SUBOPTIMAL_KHR)
    {
        recreateSwapchain();
        return;
    }
    IDK_ASSERT(acquireResult == VK_SUCCESS, "vkAcquireNextImageKHR failed");
    mImageIndex = imageIndex;

    if (mFramebuffers.size() <= imageIndex)
    {
        recreateSwapchain();
        return;
    }

    VkCommandBufferBeginInfo beginInfo {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    IDK_ASSERT(VK_SUCCESS == vkBeginCommandBuffer(mCommandBuffer, &beginInfo), "vkBeginCommandBuffer failed");

    VkClearValue clearValue {};
    clearValue.color.float32[0] = mClearColor[0];
    clearValue.color.float32[1] = mClearColor[1];
    clearValue.color.float32[2] = mClearColor[2];
    clearValue.color.float32[3] = mClearColor[3];

    VkRenderPassBeginInfo passBeginInfo {};
    passBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    passBeginInfo.renderPass = mRenderPass;
    passBeginInfo.framebuffer = mFramebuffers[imageIndex];
    passBeginInfo.renderArea.offset = {0, 0};
    passBeginInfo.renderArea.extent = mSwapchainExtent;
    passBeginInfo.clearValueCount = 1;
    passBeginInfo.pClearValues = &clearValue;

    vkCmdBeginRenderPass(mCommandBuffer, &passBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
    vkCmdEndRenderPass(mCommandBuffer);
    IDK_ASSERT(VK_SUCCESS == vkEndCommandBuffer(mCommandBuffer), "vkEndCommandBuffer failed");

    VkPipelineStageFlags waitStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    VkSubmitInfo submitInfo {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = &mImageAvailableSemaphore;
    submitInfo.pWaitDstStageMask = &waitStage;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &mCommandBuffer;
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = &mRenderFinishedSemaphore;

    IDK_ASSERT(VK_SUCCESS == vkQueueSubmit(mGraphicsQueue, 1, &submitInfo, VK_NULL_HANDLE), "vkQueueSubmit failed");

    VkPresentInfoKHR presentInfo {};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = &mRenderFinishedSemaphore;
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &mSwapchain;
    presentInfo.pImageIndices = &imageIndex;

    VkResult presentResult = vkQueuePresentKHR(mGraphicsQueue, &presentInfo);
    if (presentResult == VK_ERROR_OUT_OF_DATE_KHR || presentResult == VK_SUBOPTIMAL_KHR)
    {
        recreateSwapchain();
    }
    else
    {
        IDK_ASSERT(presentResult == VK_SUCCESS, "vkQueuePresentKHR failed");
    }
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
