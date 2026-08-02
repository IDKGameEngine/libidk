#pragma once

#include "Allocator.hpp"
#include "libidk/assert.hpp"

namespace idk
{
    template <size_t MAX_SIZE>
    class BumpAllocator: public idk::Allocator
    {
    private:
        uint8_t mData[MAX_SIZE];
        uintptr_t mBase;
        uintptr_t mEnd;
        uintptr_t mTail;

    public:
        BumpAllocator()
        :   mBase(reinterpret_cast<uintptr_t>(mData)),
            mEnd(mBase + MAX_SIZE),
            mTail(mBase)
        {
            
        }

        virtual void *alloc(size_t size, size_t a) final
        {
            uintptr_t addr = (mTail + (a - 1)) & ~(a - 1);
            mTail = addr + size;
            IDK_ASSERT(mTail <= mEnd, "Out of memory: {} / {} KB", (mTail-mBase)/1024, (mEnd-mBase)/1024);
            return reinterpret_cast<void*>(addr);
        }

        virtual void dealloc(void*) final {  }

        virtual size_t size() final { return mTail - mBase; }
    };
}

