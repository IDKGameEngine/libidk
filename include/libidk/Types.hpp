#pragma once

#include <cstddef>
#include <cstdint>
#include <mutex>

namespace idk
{
    using IdType = int64_t;
    static constexpr IdType ID_INVALID = -1;

    template <typename T, size_t N>
    using ArrayType = T[N];

    template <typename T, size_t N>
    using ArrayRef = ArrayType<T, N>&;

    template <size_t N>
    using StringType = ArrayType<char, N>;

    template <size_t N>
    using StringRef = StringType<N>&;

    struct NonCopyable
    {
        NonCopyable() = default;
        ~NonCopyable() = default;
        NonCopyable(const NonCopyable&) = delete;
        NonCopyable &operator=(const NonCopyable&) = delete;
        NonCopyable(NonCopyable&&) = default;
        NonCopyable &operator=(NonCopyable&&) = default;
    };

    struct NonMovable
    {
        NonMovable() = default;
        ~NonMovable() = default;
        NonMovable(const NonMovable&) = default;
        NonMovable &operator=(const NonMovable&) = default;
        NonMovable(NonMovable&&) = delete;
        NonMovable &operator=(NonMovable&&) = delete;
    };

    struct Immobile: private NonCopyable, private NonMovable
    {

    };


    class IEngine;

    template <typename T>
    using FuncPtr = T*;

    namespace detail
    {
        struct typeid_impl
        {
            inline static IdType value_ = 0;
        };
    }

}




template <typename T>
inline idk::IdType idk_typeid()
{
    static const idk::IdType id = idk::detail::typeid_impl::value_++;
    return id;
}

