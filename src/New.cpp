#include "libidk/memory/BumpAllocator.hpp"
#include "libidk/New.hpp"


idk::Allocator *idk::core::getStaticAllocator()
{
    static idk::BumpAllocator<LIBIDK_ALLOCATOR_SIZE> instance;
    return &instance;
}
