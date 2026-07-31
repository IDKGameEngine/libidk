#pragma once

#include <cstddef>
#include <cstdint>
#include <stdalign.h>

namespace idk::core::detail
{
    class BumpAllocatorImpl
    {
    public:
        BumpAllocatorImpl(void *base, size_t size);
        void *Alloc(size_t size, size_t a);
        void *Calloc(size_t size, size_t a);

    private:
        uintptr_t mBase;
        uintptr_t mEnd;
        uintptr_t mTail;
    };

    class PoolAllocatorImpl
    {
    public:

    private:
        
    };
}


