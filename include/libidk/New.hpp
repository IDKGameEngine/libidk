#pragma once

#include "libidk/memory/Allocator.hpp"
#include "libidk/metric.hpp"

namespace idk
{
    namespace core
    {
        static constexpr size_t LIBIDK_ALLOCATOR_SIZE = 256 * idk::KILO;
        idk::Allocator *getStaticAllocator();
    }

    template <typename T, typename... Args>
    static inline T *New(Args&&... args)
    {
        void *ptr = core::getStaticAllocator()->alloc(sizeof(T), alignof(T));
        return new (ptr) T(args...);
    }

    template <typename T>
    static inline T *NewArray(size_t count)
    {
        void *ptr = core::getStaticAllocator()->alloc(count*sizeof(T), alignof(T));
        return reinterpret_cast<T*>(ptr);
    }

    static inline float GetAllocatorMemoryUsage()
    {
        size_t size = core::getStaticAllocator()->size();
        return float(size) / core::LIBIDK_ALLOCATOR_SIZE;
    }
}
