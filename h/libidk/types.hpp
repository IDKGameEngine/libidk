#pragma once

#include <cstddef>
#include <cstdint>
#include <mutex>

namespace idk
{
    using IdType = int64_t;
    static constexpr IdType ID_INVALID = -1;

    class NonCopyable;
    class NonMovable;
    class NonMobile;

    template <typename T, size_t N>
    using ArrayRefType = T(&)[N];

    template <typename T>
    class ThreadSafeAccess;

    class IEngine;

    enum class EngineStat: uint8_t
    {
        Invalid = 0,
        Alive,
        Dead,
        Starting,
        Stopping,
    };

    enum class EngineCtrl: uint8_t
    {
        None = 0,
        Start,
        Stop
    };


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



class idk::NonCopyable
{
public:
    NonCopyable() = default;
    ~NonCopyable() = default;
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable &operator=(const NonCopyable&) = delete;
    NonCopyable(NonCopyable&&) = default;
    NonCopyable &operator=(NonCopyable&&) = default;
};


class idk::NonMovable
{
public:
    NonMovable() = default;
    ~NonMovable() = default;
    NonMovable(const NonMovable&) = default;
    NonMovable &operator=(const NonMovable&) = default;
    NonMovable(NonMovable&&) = delete;
    NonMovable &operator=(NonMovable&&) = delete;
};

class idk::NonMobile: private idk::NonCopyable, private idk::NonMovable { };



template <typename T>
inline idk::IdType idk_typeid()
{
    static const idk::IdType id = idk::detail::typeid_impl::value_++;
    return id;
}

