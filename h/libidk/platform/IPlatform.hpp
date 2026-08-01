#pragma once

#include "libidk/Types.hpp"

namespace idk::platform
{
    uint64_t getSysTimeMs();
    uint64_t getSysTimeNs();

    // class IWindow: public idk::Immobile
    // {
    // public:
    //     IWindow() {  };
    //     virtual ~IWindow() = default;

    //     virtual int   getWidth() const = 0;
    //     virtual int   getHeight() const = 0;
    //     virtual void *getNativeHandle() const = 0;
    //     virtual void *getGpuContext() const { return nullptr; };
    //     virtual void  makeCurrent() = 0;
    //     virtual void  swapWindow() = 0;
    //     virtual void  showWindow(bool show) = 0;
    //     virtual void  setWindowResolution(int w, int h) = 0;
    //     virtual void  setRenderResolution(int w, int h) = 0;

    // };

    class Window;

    class IPlatform: public idk::Immobile
    {
    public:
        virtual ~IPlatform() = default ;
        virtual void update(idk::IEngine*) = 0;
        virtual void shutdown(idk::IEngine*) = 0;
        virtual Window *getWindow() { return nullptr; }
    };

}

