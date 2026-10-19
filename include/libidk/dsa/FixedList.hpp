#pragma once
#include <cstddef>
#include <type_traits>

namespace idk
{
    // A checked, bounded list for plain records. No constructors/destructors are
    // manually layered over live objects and no allocation is performed.
    template<class T, size_t Capacity>
    class FixedList
    {
        static_assert(Capacity > 0 && std::is_trivially_copyable_v<T>);
        T mData[Capacity]{};
        size_t mSize = 0;
    public:
        bool push(const T& value) { if (full()) return false; mData[mSize++] = value; return true; }
        void clear() { mSize = 0; }
        void truncate(size_t count) { if (count < mSize) mSize = count; }
        bool empty() const { return mSize == 0; }
        bool full() const { return mSize == Capacity; }
        size_t size() const { return mSize; }
        static constexpr size_t capacity() { return Capacity; }
        T* data() { return mData; }
        const T* data() const { return mData; }
        T* begin() { return mData; }
        T* end() { return mData + mSize; }
        const T* begin() const { return mData; }
        const T* end() const { return mData + mSize; }
        T& operator[](size_t index) { return mData[index]; }
        const T& operator[](size_t index) const { return mData[index]; }
    };
}
