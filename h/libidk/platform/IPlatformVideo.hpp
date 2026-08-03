#pragma once

#include "PlatformContext.hpp"


namespace idk
{
    class IPlatformVideo: public IPlatformFeature
    {
    private:

    public:
        virtual void *getImpl() = 0;
        virtual int   getWidth() = 0;
        virtual int   getHeight() = 0;
        virtual void  setWindowVisibility(bool visible) = 0;
        virtual void  setWindowResolution(int w, int h) = 0;
        virtual void  setRenderResolution(int w, int h) = 0;

    };
}
