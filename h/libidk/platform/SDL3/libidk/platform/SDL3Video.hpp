#pragma once

#include "libidk/platform/IPlatformVideo.hpp"
#define VK_NO_PROTOTYPES
#include <vulkan/vulkan.h>

#include <vector>

namespace idk
{
    class SDL3Video: public IPlatformVideo
    {
    private:
        const char      *mTitle;
        void            *mWin;
        VkInstance       mInstance;
        VkSurfaceKHR     mSurface;
        VkPhysicalDevice mPhysicalDevice;
        VkDevice         mDevice;
        VkQueue          mGraphicsQueue;
        VkSwapchainKHR   mSwapchain;
        VkRenderPass     mRenderPass;
        VkCommandPool    mCommandPool;
        VkCommandBuffer  mCommandBuffer;
        VkSemaphore      mImageAvailableSemaphore;
        VkSemaphore      mRenderFinishedSemaphore;
        VkFormat         mSwapchainFormat;
        VkExtent2D       mSwapchainExtent;
        std::vector<VkImage>      mSwapImages;
        std::vector<VkImageView>  mSwapImageViews;
        std::vector<VkFramebuffer> mFramebuffers;
        uint32_t         mImageIndex;
        int32_t          mWidth;
        int32_t          mHeight;
        float            mClearColor[4];

        void createLogicalDevice();
        void createSwapchain();
        void createRenderPass();
        void createFramebuffers();
        void createCommandBuffer();
        void recreateSwapchain();

    public:
        SDL3Video(const char *title, int w, int h);
        ~SDL3Video();

        void setClearColor(float r, float g, float b, float a);
        void getClearColor(float &r, float &g, float &b, float &a) const;

        virtual void  update(PlatformContext&) final;

        virtual void *getImpl()   final { return mWin; }
        virtual int   getWidth()  final { return mWidth; };
        virtual int   getHeight() final { return mHeight; };

        virtual void  setWindowVisibility(bool visible) final;
        virtual void  setWindowResolution(int w, int h) final;
        virtual void  setRenderResolution(int w, int h) final;

    };
}

