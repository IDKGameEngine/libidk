#pragma once

#include <cstddef>
#include <cstdint>
#include <stdalign.h>

namespace idk
{
    class BumpAllocator
    {
    public:
        BumpAllocator(size_t capacity);
        void  reset();
        void *alloc(size_t size, size_t alignment = alignof(max_align_t));

    private:
        const size_t mCap;
        const uint8_t *mBase;
        const uint8_t *mEnd;
        uint8_t *mTop;

    };
}


