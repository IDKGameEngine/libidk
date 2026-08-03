#pragma once

#include "PlatformContext.hpp"
#include "libidk/dsa/List.hpp"
// #include "libidk/dsa/Observer.hpp"


namespace idk
{
    class IPlatformEvents: public IPlatformFeature // , public idk::Observable
    {
    protected:
        static constexpr size_t MAX_CALLBACKS = 64;
        using EventCallback = void (*)(IPlatformEvents*, void *event);
        idk::InplaceList<uintptr_t, MAX_CALLBACKS> mEventFuncs;

    public:

        bool addEventCallback(EventCallback func)
        {
            if (!mEventFuncs.full())
            {
                mEventFuncs.push(reinterpret_cast<uintptr_t>(func));
                return true;
            }
            return false;
        }

    };
}

