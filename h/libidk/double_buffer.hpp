#pragma once

#include "libidk/types.hpp"

#include <cstddef>
#include <cstdint>
#include <cstring>

#include <atomic>
#include <array>
#include <mutex>
#include <queue>


namespace idk
{
    template <typename T> class DoubleBuffer;
    template <typename T> class DblBufferWriter;
    template <typename T> class DblBufferReader;
}



template <typename T>
class idk::DoubleBuffer: public idk::NonCopyable, public idk::NonMovable
{
private:
public:
    friend class DblBufferWriter<T>;
    friend class DblBufferReader<T>;

    std::mutex    mutex_;
    std::queue<T> data_[2];
    int           widx_;
    int           ridx_;

    auto *wtget()
    {
        std::lock_guard<std::mutex> lock(mutex_);
        return &data_[widx_];
    }

    auto *rdget()
    {
        std::lock_guard<std::mutex> lock(mutex_);
        return &data_[ridx_];
    }

    explicit DoubleBuffer()
    :   widx_(0), ridx_(1)
    {

    }

    void swapBuffers()
    {
        std::lock_guard<std::mutex> lock(mutex_);
        std::swap(widx_, ridx_);
    }

};


template <typename T>
class idk::DblBufferWriter
{
private:
    DoubleBuffer<T> &buf_;

public:
    explicit DblBufferWriter(DoubleBuffer<T> &buf): buf_(buf) {  }
    auto *operator->() { return buf_.wtget(); }
};


template <typename T>
class idk::DblBufferReader
{
private:
    DoubleBuffer<T> &buf_;

public:
    explicit DblBufferReader(DoubleBuffer<T> &buf): buf_(buf) {  }
    auto *operator->() { return buf_.rdget(); }
};
