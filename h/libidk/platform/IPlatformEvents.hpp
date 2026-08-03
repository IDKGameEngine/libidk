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

