#pragma once

#include "libidk/platform/IVideoBackend.hpp"

#define VK_NO_PROTOTYPES
#include <vulkan/vulkan.h>

#include <vector>


namespace idk
{
    class VideoBackend: public idk::IVideoBackend
    {
    private:
        const char      *mTitle;
        void            *mWin;
        VkInstance       mInstance;
        // VkSurfaceKHR     mSurface;
        // VkPhysicalDevice mPhysicalDevice;
        // VkDevice         mDevice;
        // VkQueue          mGraphicsQueue;
        int32_t          mWidth;
        int32_t          mHeight;

    public:
        VideoBackend(const char *title, int w, int h);
        ~VideoBackend();

        virtual void  update(idk::IPlatformContext*) final;

        virtual int   getWidth()  final { return mWidth; };
        virtual int   getHeight() final { return mHeight; };

        virtual void  setWindowVisibility(bool visible) final;
        virtual void  setWindowResolution(int w, int h) final;
        virtual void  setRenderResolution(int w, int h) final;

    };
}

