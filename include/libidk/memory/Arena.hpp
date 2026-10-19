#pragma once

#include "Allocator.hpp"
#include <limits>
#include <new>
#include <span>
#include <type_traits>

namespace idk
{
    // Non-owning, bounded storage. Failed allocations leave the cursor unchanged.
    // Caller controls storage lifetime and reclamation; no heap fallback.
    class Arena final : public Allocator
    {
        std::span<std::byte> mStorage;
        size_t mUsed = 0;
        bool mFailed = false;
    public:
        explicit Arena(std::span<std::byte> storage) : mStorage(storage) {}
        void* alloc(size_t bytes, size_t alignment = 8) override
        {
            if (!alignment || (alignment & (alignment - 1))) { mFailed = true; return nullptr; }
            uintptr_t address = reinterpret_cast<uintptr_t>(mStorage.data()) + mUsed;
            size_t padding = (alignment - (address & (alignment - 1))) & (alignment - 1);
            if (padding > mStorage.size() - mUsed || bytes > mStorage.size() - mUsed - padding)
            { mFailed = true; return nullptr; }
            mUsed += padding + bytes;
            return mStorage.data() + mUsed - bytes;
        }
        void dealloc(void*) override {}
        size_t size() override { return mUsed; }
        size_t mark() const { return mUsed; }
        size_t capacity() const { return mStorage.size(); }
        bool failed() const { return mFailed; }
        bool overlaps(const Arena& other) const
        {
            if (mStorage.empty() || other.mStorage.empty()) return false;
            uintptr_t begin = reinterpret_cast<uintptr_t>(mStorage.data());
            uintptr_t otherBegin = reinterpret_cast<uintptr_t>(other.mStorage.data());
            return begin < otherBegin + other.mStorage.size() && otherBegin < begin + mStorage.size();
        }
        void rewind(size_t mark) { if (mark <= mUsed) mUsed = mark; mFailed = false; }
        void reset() { mUsed = 0; mFailed = false; }

        template<class T> std::span<T> array(size_t count)
        {
            static_assert(std::is_trivially_destructible_v<T>);
            if (!count) return {};
            if (count > std::numeric_limits<size_t>::max() / sizeof(T)) { mFailed = true; return {}; }
            auto* data = static_cast<T*>(alloc(count * sizeof(T), alignof(T)));
            if (!data) return {};
            for (size_t i = 0; i < count; ++i) new (data + i) T{};
            return {data, count};
        }
    };
}
