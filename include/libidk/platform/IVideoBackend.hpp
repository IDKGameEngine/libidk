#pragma once

#include "IPlatformFeature.hpp"


namespace idk
{
    class IVideoBackend: public idk::IPlatformFeature
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
