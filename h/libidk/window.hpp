#pragma once

#include "libidk/types.hpp"
#include <memory>

namespace idk::core
{
    struct WindowDesc
    {
        const char *title;
        int width;
        int height;
    };

    class IWindow: public idk::NonCopyable
    {
    public:
        virtual ~IWindow() = default;
        virtual int   getWidth() const = 0;
        virtual int   getHeight() const = 0;
        virtual void *getNativeHandle() const = 0;
        virtual void *getGpuContext() const = 0;
        virtual void  pollEvents() = 0;
    };

    using IWindowPtr = std::shared_ptr<idk::core::IWindow>;

}
