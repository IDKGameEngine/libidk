#pragma once

#include <stddef.h>

namespace idk::util
{
    template <typename T>
    inline constexpr T align_down( T value, size_t alignment )
    {
        return T(uintptr_t(value) & ~(alignment-1));
    }

    template <typename T>
    inline constexpr T align_up( T value, size_t alignment )
    {
        return T((uintptr_t(value) + alignment-1) & ~(alignment-1));
    }

}

