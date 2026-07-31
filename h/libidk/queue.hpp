#pragma once

#include "libidk/assert.hpp"
#include "libidk/log.hpp"
#include "libidk/types.hpp"

#include <atomic>

namespace idk::core
{
    template <typename T, uint32_t MAX_SIZE>
    class Queue;
}


template <typename T, uint32_t MAX_SIZE>
class idk::core::Queue : public idk::NonCopyable, public idk::NonMovable
{
private:
    T mData[MAX_SIZE];
    uint32_t rIdx;
    uint32_t wIdx;
    std::mutex mMutex;

public:
    Queue(): rIdx(0), wIdx(0) {  }

    bool push(const T &value)
    {
        std::lock_guard<std::mutex> lock(mMutex);
        uint32_t wNext = (wIdx + 1) % MAX_SIZE;
        if (wNext == rIdx)
        {
            return false;
        }
        new (&mData[wIdx]) T(value);
        wIdx = wNext;
        return true;
    }

    bool pop(T &value)
    {
        std::lock_guard<std::mutex> lock(mMutex);
        if (rIdx == wIdx)
        {
            return false;
        }
        value = mData[rIdx];
        mData[rIdx].~T();
        rIdx = (rIdx + 1) % MAX_SIZE;
        return true;
    }

    // size_t size()       { return static_cast<size_t>(mSize.load()); }
    // size_t size() const { return static_cast<size_t>(mSize.load()); }

    // bool empty()       { return (this->size() == 0); }
    // bool empty() const { return (this->size() == 0); }

    // bool full()        { return (this->size() == MAX_SIZE); }
    // bool full()  const { return (this->size() == MAX_SIZE); }
};
