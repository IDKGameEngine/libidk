#pragma once

#include <cstdint>
#include <cstddef>

namespace idk
{
    template <typename T, typename U>
    auto min(T x, U y)
    {
        return (x < y) ? x : y;
    }

    template <typename T, typename U>
    auto max(T x, U y)
    {
        return (x > y) ? x : y;
    }

    template <typename T>
    T clamp(T x, T lo, T hi)
    {
        return idk::max(lo, idk::min(x, hi));
    }

    template <typename T>
    T lerp(T x, T y, T a)
    {
        return x + a*(y - x);
    }
}

