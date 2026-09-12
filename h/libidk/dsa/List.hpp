#pragma once

#include "libidk/Assert.hpp"
#include "libidk/log.hpp"
#include "libidk/Types.hpp"
#include <cstdint>

namespace idk
{
    template <typename T>
    class List: public idk::Immobile
    {
    private:
        T     *mData;
        size_t mTop;
        size_t mEnd;

    public:
        List(T *buf, size_t size): mData(buf), mTop(0), mEnd(size) {  }

        void push(const T &value)
        {
            IDK_ASSERT(mTop < mEnd, "[idk::List] Buffer overflow");
            new (&mData[mTop++]) T(value);
        }

        void pop()
        {
            IDK_ASSERT(mTop > 0, "[idk::List] Buffer underflow");
            mData[--mTop].~T();
        }

        T &front()
        {
            IDK_ASSERT(mTop > 0, "[idk::List] Buffer empty");
            return mData[0];
        }

        T &back()
        {
            IDK_ASSERT(mTop > 0, "[idk::List] Buffer empty");
            return mData[mTop-1];
        }

        T &operator[](size_t idx)
        {
            IDK_ASSERT(0<=idx && idx<mEnd, "[idk::List] Index out of bounds");
            return mData[idx];
        }

        const T &operator[](size_t idx) const
        {
            IDK_ASSERT(0<=idx && idx<mEnd, "[idk::List] Index out of bounds");
            return mData[idx];
        }

        void *data() { return mData; }
        const void *data() const { return mData; }

        size_t size()       { return mTop; }
        size_t size() const { return mTop; }

        bool empty()       { return (mTop == 0); }
        bool empty() const { return (mTop == 0); }

        bool full()        { return (mTop == mEnd); }
        bool full()  const { return (mTop == mEnd); }

        struct Iterator
        {
            T *mPtr;
            Iterator(T *ptr): mPtr(ptr) {  }
            Iterator(const Iterator &I): mPtr(I.mPtr) {  }
            Iterator &operator++() { mPtr += 1; return *this; }
            Iterator operator++(int) { return Iterator(mPtr + 1); }
            bool operator==(const Iterator &rhs) { return mPtr == rhs.mPtr; }
            bool operator!=(const Iterator &rhs) { return mPtr != rhs.mPtr; }
            T &operator*() { return *mPtr; };
        };

        Iterator begin() { return Iterator(mData); }
        Iterator end()   { return Iterator(mData + mTop); }
    };


    template <typename T, uint32_t MaxSize>
    class InplaceList: public idk::List<T>
    {
    private:
        T mBuf[MaxSize];

    public:
        InplaceList(): idk::List<T>(mBuf[0], int(MaxSize)) {  };

    };

}
