#pragma once

#include "libidk/Assert.hpp"
#include "libidk/log.hpp"
#include "libidk/Types.hpp"
#include <cstdint>

namespace idk
{
    template <typename T>
    class Stack: public idk::Immobile
    {
    private:
        T        *mData;
        int       mTop;
        const int mEnd;

    public:
        using ValueType = T;

        Stack(T *buf, int size): mData(buf), mTop(0), mEnd(size) {  }

        void push(const T &value)
        {
            IDK_ASSERT(mTop < mEnd, "Stack overflow");
            new (&mData[mTop++]) T(value);
        }

        void pop()
        {
            IDK_ASSERT(mTop > 0, "Stack underflow");
            mData[--mTop].~T();
        }

        T &top()
        {
            IDK_ASSERT(mTop > 0, "Stack empty");
            return mData[mTop-1];
        }

        void remove(int idx)
        {
            IDK_ASSERT(0<=idx && idx<mEnd, "Index out of bounds");
            std::swap(mData[idx], this->top());
            this->pop();
        }

        T &operator[](size_t idx)
        {
            IDK_ASSERT(0<=idx && idx<mEnd, "Index out of bounds");
            return mData[idx];
        }

        const T &operator[](size_t idx) const
        {
            IDK_ASSERT(0<=idx && idx<mEnd, "Index out of bounds");
            return mData[idx];
        }

        void *data() { return mData; }
        const void *data() const { return mData; }

        int size()       { return mTop; }
        int size() const { return mTop; }

        bool empty()       { return (mTop == 0); }
        bool empty() const { return (mTop == 0); }

        bool full()        { return (mTop == mEnd); }
        bool full()  const { return (mTop == mEnd); }
    };

    template <typename T, int Capacity>
    class InplaceStack: public idk::Stack<T>
    {
    private:
        T mBuf[Capacity];

    public:
        InplaceStack(): Stack<T>(&mBuf[0], Capacity) {  };

    };

}
