#pragma once

#include "libidk/dsa/Queue.hpp"

namespace idk
{
    template <typename T, size_t N>
    class EventBus: public idk::Immobile
    {
    private:
        idk::core::Queue<T, N> mQueue;

    public:
        bool pushEvent(const T &event)
        {
            return mQueue.push(event);
        }

        bool popEvent(T &event)
        {
            return mQueue.pop(event);
        }

    };
}

