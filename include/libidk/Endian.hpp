#pragma once

#include <cstddef>
#include <cstdint>
#include <type_traits>


namespace idk::endian
{
    template <typename T>
    T hostToNet(T x)
    {
        static_assert(std::is_arithmetic_v<T>, "Value must be arithmetic");
        uint8_t *p = reinterpret_cast<uint8_t*>(&x);
        for (size_t i=0; i<sizeof(T)/2; i++)
        {
            uint8_t tmp = p[i];
            p[i] = p[sizeof(T)-1-i];
            p[sizeof(T)-1-i] = tmp;
        }
        return x;
    }

    template <typename T>
    T netToHost(T x)
    {
        static_assert(std::is_arithmetic_v<T>, "Value must be arithmetic");
        uint8_t *p = reinterpret_cast<uint8_t*>(&x);
        for (size_t i=0; i<sizeof(T)/2; i++)
        {
            uint8_t tmp = p[i];
            p[i] = p[sizeof(T)-1-i];
            p[sizeof(T)-1-i] = tmp;
        }
        return x;
    }

}
