#pragma once

#include "libidk/Types.hpp"

namespace idk
{
    class Allocator: public idk::Immobile
    {
    public:
        virtual ~Allocator() = default;
        virtual void *alloc(size_t size, size_t align=8) = 0;
        virtual void dealloc(void*) = 0;
        virtual size_t size() = 0;
    };
}
