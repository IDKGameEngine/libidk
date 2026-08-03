#pragma once

#include "PlatformContext.hpp"
#include "libidk/dsa/List.hpp"


namespace idk
{
    class IPlatformEvents: public IPlatformFeature
    {
    protected:
        static constexpr size_t MAX_CALLBACKS = 64;
        using EventCallback = void (*)(IPlatformEvents*, void *event);
        InplaceList<EventCallback, MAX_CALLBACKS> mEventFuncs;

    public:
        IPlatformEvents() {  };

        bool addEventCallback(EventCallback func)
        {
            if (mEventFuncs.full()) { return false; }
            mEventFuncs.push(func);
        }

    };
}

