#pragma once

#include "libidk/metric.hpp"
#include "libidk/Allocator.hpp"

namespace idk
{
    namespace core
    {
        static constexpr size_t LIBIDK_ALLOCATOR_SIZE = 256 * idk::KILO;

        static inline BumpAllocator<LIBIDK_ALLOCATOR_SIZE> &getStaticAllocator()
        {
            static BumpAllocator<LIBIDK_ALLOCATOR_SIZE> instance;
            return instance;
        }
    }

    template <typename T, typename... Args>
    static inline T *New(Args&&... args)
    {
        return core::getStaticAllocator().New<T>(args...);
    }

    template <typename T>
    static inline T *NewArray(size_t count)
    {
        void *ptr = core::getStaticAllocator().Alloc(count*sizeof(T), alignof(T));
        return reinterpret_cast<T*>(ptr);
    }
}
