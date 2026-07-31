#pragma once

#include "libidk/Types.hpp"

namespace idk::platform
{
    class Window: public idk::Immobile
    {
    public:
        Window() {  };
        virtual ~Window() = default;

        virtual int   getHeight() const = 0;
        virtual void *getNativeHandle() const = 0;
        virtual void *getGpuContext() const = 0;
        virtual void  pollEvents() = 0;
        virtual void  makeCurrent() = 0;
        virtual void  swapWindow() = 0;
        virtual void  showWindow(bool show) = 0;
        virtual void  setWindowResolution(int w, int h) = 0;
        virtual void  setRenderResolution(int w, int h) = 0;

    };
}
