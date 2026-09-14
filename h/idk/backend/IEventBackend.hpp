#pragma once

#include "IBackendContext.hpp"
#include "libidk/dsa/List.hpp"


namespace idk
{
    class IEventBackend: public idk::IBackendFeature // , public idk::Observable
    {
    protected:
        static constexpr size_t MAX_CALLBACKS = 64;
        using EventCallback = void (*)(IEventBackend*, void *event);
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

