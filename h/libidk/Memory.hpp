#pragma once

#include "libidk/StdMem.hpp"

namespace idk::core
{
    template <typename T>
    T unalignedLoad(const void *src)
    {
        static T value;
        idk_memcpy(&value, src, sizeof(T));
        return value;
    }

    template <typename T>
    void unalignedStore(void *dst, const T &value)
    {
        idk_memcpy(dst, &value, sizeof(T));
    }
}
