#pragma once

#include "libidk/types.hpp"


namespace idk
{
    template <typename T>
    struct RaiiFunc;

    template <typename R, typename... Args>
    struct RaiiFunc<R(Args...)>
    {
        RaiiFunc(idk::FuncPtr<R(Args...)> func, Args&&... args)
        {
            func(std::forward<Args>(args)...);
        }
    };
}
