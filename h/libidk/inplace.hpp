#pragma once
#include <cstdint>
#include "idk/core/log.hpp"

namespace idk
{
    template <typename DataType, uint32_t Capacity>
    class inplace_stack;

    template <typename DataType, uint32_t Capacity>
    class inplace_list;

    template <typename DataType, uint32_t Capacity>
    class inplace_queue;

    template <typename T, uint32_t Capacity>
    class inplace_set;
}



template <typename DataType, uint32_t Capacity>
class idk::inplace_stack
{
private:
    DataType  mData[Capacity];
    int32_t   mTop;
    const int mEnd;

public:
    inplace_stack()
    :   mTop(0), mEnd(int(Capacity)) {  }

    void push(const DataType &value)
    {
        if (mTop < mEnd)
            new (&mData[mTop++]) DataType(value);
        else
            VLOG_WARN("Attempted push onto full stack");
    }

    void pop()
    {
        if (mTop > 0)
            mData[--mTop].~DataType();
        else
            VLOG_WARN("Attempted pop from empty stack");
    }

    DataType &top()
    {
        if (mTop > 0)
            return mData[mTop-1];
        else
            VLOG_WARN("Attempted to read top of empty stack");
    }

    void remove(int idx)
    {
        if (!(0<=idx && idx<Capacity))
        {
            VLOG_WARN("Index out of bounds");
            return;
        }

        std::swap(mData[idx], this->top());
        this->pop();
    }

    DataType &operator[](size_t idx)
    {
        if (0<=idx && idx<Capacity)
            return mData[idx];
        else
            VLOG_WARN("Index out of bounds");
    }

    const DataType &operator[](size_t idx) const
    {
        if (0<=idx && idx<Capacity)
            return mData[idx];
        else
            VLOG_WARN("Index out of bounds");
    }

    const int size()       { return mTop; }
    const int size() const { return mTop; }

    const bool empty()       { return (mTop == 0); }
    const bool empty() const { return (mTop == 0); }

    const bool full()        { return (mTop == mEnd); }
    const bool full()  const { return (mTop == mEnd); }
};



template <typename DataType, uint32_t Capacity>
class idk::inplace_list
{
private:
    DataType  mData[Capacity];
    int32_t   mTop;
    const int mEnd;

public:
    inplace_list()
    :   mTop(0), mEnd(int(Capacity))
    {
        
    }

    void push_back(const DataType &value)
    {
    #ifdef IDK_DEBUG
        if (!(mTop < mEnd))
        {
            vnlog(LogType::ERROR, "idk::inplace_list<DataType, %u>::push_back: Buffer Overflow!\n");
            return;
        }
    #endif
        new (&mData[mTop++]) DataType(value);
    }

    void pop_back()
    {
    #ifdef IDK_DEBUG
        if (!(0 < mTop))
        {
            vnlog(LogType::ERROR, "idk::inplace_list<DataType, %u>::pop_back: Buffer Underflow!\n");
            return;
        }
    #endif
        mData[--mTop]->~DataType(); // manual destructor
    }

    DataType &back()
    {
    #ifdef IDK_DEBUG
        if (!(mTop == 0))
        {
            vnlog(LogType::ERROR, "idk::inplace_list<DataType, %u>::back: Invalid Reference!\n");
        }
    #endif
        return mData[mTop-1];
    }


    DataType &operator[]( int idx )
    {
    #ifdef IDK_DEBUG
        if (!(0<=idx && idx<mCap))
        {
            vnlog(LogType::ERROR, "idk::inplace_list<DataType, %u>::operator[]: Invalid Index!\n");
        }
    #endif
        return mData[idx];
    }

    const DataType &operator[]( int idx ) const
    {
    #ifdef IDK_DEBUG
        if (!(0<=idx && idx<mCap))
        {
            vnlog(LogType::ERROR, "idk::inplace_list<DataType, %u>::operator[]: Invalid Index!\n");
        }
    #endif
        return mData[idx];
    }

    const int size()       { return mTop; }
    const int size() const { return mTop; }
};





template <typename DataType, uint32_t Capacity>
class idk::inplace_queue: private idk::NonCopyable
{
private:
    DataType mData[Capacity];
    int32_t  mSize, mRdIdx, mWtIdx;

public:
    inplace_queue()
    :   mSize(0), mRdIdx(0), mWtIdx(0)
    {
        
    }

    void push(const DataType &value)
    {
        if (mSize < Capacity)
        {
            new (&mData[mWtIdx]) DataType(value);
            mWtIdx = (mWtIdx+1) % Capacity;
            mSize += 1;
        }
    }

    void pop()
    {
        if (mSize > 0)
        {
            mData[mRdIdx].~DataType();
            mRdIdx = (mRdIdx+1) % Capacity;
            mSize -= 1;
        }
    }

    DataType &front()
    {
    #ifdef IDK_DEBUG
        if (mSize == 0)
            VLOG_ERROR("Attempted to read front of empty queue");
    #endif
        return mData[mRdIdx];
    }

    DataType &back()
    {
    #ifdef IDK_DEBUG
        if (mSize == 0)
            VLOG_ERROR("Attempted to read back of empty queue");
    #endif
        return mData[mWtIdx-1];
    }

    DataType pop_front()
    {
        DataType value(this->front());  this->pop();
        return value;
    }

    int size()       { return mSize; }
    int size() const { return mSize; }

    bool empty()       { return (mSize == 0); }
    bool empty() const { return (mSize == 0); }

    bool full()        { return (mSize == Capacity); }
    bool full()  const { return (mSize == Capacity); }

};




template <typename T, uint32_t Capacity>
class idk::inplace_set: private idk::NonCopyable
{
private:
    T  mBase[Capacity];
    T *mTail;
    const T *mEnd;

public:
    using pos_t = T*;
    pos_t nopos = nullptr;

    inplace_set(): mTail(mBase), mEnd(mBase + Capacity) {  }

    pos_t find(const T&);
    pos_t insert(const T&);
    void  erase(const T&);

    T &operator[](int);
    const T &operator[](int) const;

    int  size()  const { return int(mTail - mBase); }
    bool empty() const { return (mBase == mTail); }
    bool full()  const { return (mTail == mEnd); }

};

template <typename T, uint32_t N>
idk::inplace_set<T, N>::pos_t idk::inplace_set<T, N>::find(const T &value)
{
    for (T *p=mBase; p<mTail; p++)
        if (*p == value)
            return pos_t(p);
    return inplace_set::nopos;
}

template <typename T, uint32_t N>
idk::inplace_set<T, N>::pos_t idk::inplace_set<T, N>::insert(const T &v)
{
    if (full()) return inplace_set::nopos;

    T p = find(v);
    if (p != inplace_set::nopos)
        return p;
    
    return new (mTail++) T(v);
}

template <typename T, uint32_t N>
void idk::inplace_set<T, N>::erase(const T &v)
{
    if (T *p = find(v))
    {
        std::swap(*p, *(mTail-1));
        (--mTail)->~T();
    }
}

template <typename T, uint32_t N>
T &idk::inplace_set<T, N>::operator[]( int idx )
{
    IDK_ASSERT((0<=idx && idx<N), "Out of bounds access");
    return mBase[idx];
}


template <typename T, uint32_t N>
const T &idk::inplace_set<T, N>::operator[]( int idx ) const
{
    IDK_ASSERT((0<=idx && idx<N), "Out of bounds access");
    return mBase[idx];
}
