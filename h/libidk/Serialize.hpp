#pragma once

#include "libidk/stdmem.hpp"
#include <cstddef>
#include <cstdint>

namespace idk
{
    class ByteEncoder
    {
    private:
        uint8_t *const mBase;
        uint8_t *const mEnd;
        uint8_t *mTail;

    public:
        ByteEncoder(void *dstBuf, size_t dstSize)
        :   mBase((uint8_t*)dstBuf), mEnd(mBase+dstSize), mTail(mBase) {  }

        uint8_t *getBase() { return mBase; }
        uint8_t *getEnd()  { return mEnd; }
        uint8_t *getTail() { return mTail; }
        size_t   getSize() { return mTail - mBase; }

        bool write(const void *buf, size_t size)
        {
            if (mTail + size <= mEnd)
            {
                idk_memcpy(mTail, buf, size);
                mTail += size;
                return true;
            }
            return false;
        }

        template <typename T>
        bool write(const T &x) { return this->write(&x, sizeof(T)); }

    };


    class ByteDecoder
    {
    private:
        const uint8_t *const mBase;
        const uint8_t *const mEnd;
        const uint8_t *mTail;

    public:
        ByteDecoder(const void *srcBuf, size_t srcSize)
        :   mBase((const uint8_t*)srcBuf), mEnd(mBase+srcSize), mTail(mBase) {  }

        const uint8_t *getBase() const { return mBase; }
        const uint8_t *getEnd()  const { return mEnd; }
        const uint8_t *getTail() const { return mTail; }

        bool read(void *buf, size_t size)
        {
            if (mTail + size <= mEnd)
            {
                idk_memcpy(buf, mTail, size);
                mTail += size;
                return true;
            }
            return false;
        }

        template <typename T>
        bool read(T &x) { return this->read(&x, sizeof(T)); }

        template <typename T>
        T read()
        {
            T data;
            this->read(data);
            return data;
        }
    };
}
