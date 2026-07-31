#pragma once

#include <cstddef>
#include <cstdint>
#include <stdalign.h>

#include "libidk/detail/AllocatorImpl.hpp"

namespace idk::core
{
    template <size_t MAX_SIZE>
    class BumpAllocator: public detail::BumpAllocatorImpl
    {
    private:
        using Impl = BumpAllocatorImpl;
        uint8_t mData[MAX_SIZE];

    public:
        BumpAllocator(): BumpAllocatorImpl(&mData[0], MAX_SIZE) {};

        template <typename T, typename... Args>
        T *New(Args&&... args)
        {
            void *ptr = this->Alloc(sizeof(T), alignof(T));
            return new (ptr) T(args...);
        }

    };
}

