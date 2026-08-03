#pragma once

#include "libidk/Assert.hpp"
#include "libidk/log.hpp"
#include "libidk/Types.hpp"
#include <cstdint>

namespace idk
{
    template <typename DataType>
    class Stack: public idk::Immobile
    {
    private:
        DataType *mData;
        int       mTop;
        const int mEnd;

    public:
        using ValueType = DataType;

        Stack(DataType *buf, int size): mData(buf), mTop(0), mEnd(size) {  }

        void push(const DataType &value)
        {
            IDK_ASSERT(mTop < mEnd, "Stack overflow");
            new (&mData[mTop++]) DataType(value);
        }

        void pop()
        {
            IDK_ASSERT(mTop > 0, "Stack underflow");
            mData[--mTop].~DataType();
        }

        DataType &top()
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

        DataType &operator[](size_t idx)
        {
            IDK_ASSERT(0<=idx && idx<mEnd, "Index out of bounds");
            return mData[idx];
        }

        const DataType &operator[](size_t idx) const
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


    template <typename DataType, uint32_t Capacity>
    class InplaceStack: public idk::Stack<DataType>
    {
    private:
        DataType mBuf[Capacity];

    public:
        InplaceStack(): Immobile(), Stack<DataType>(&mBuf, Capacity) {  };

    };

}
