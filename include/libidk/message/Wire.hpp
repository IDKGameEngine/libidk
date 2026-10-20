#pragma once

#include <bit>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <limits>

namespace idk
{
    // Fixed-width, big-endian fields. Never serialize C++ object layouts.
    class WireWriter
    {
        uint8_t *mData;
        size_t mCapacity, mSize = 0;
        bool mGood = true;

    public:
        WireWriter(void *data, size_t capacity): mData(static_cast<uint8_t *>(data)), mCapacity(capacity) {}
        bool good() const { return mGood; }
        size_t size() const { return mSize; }
        void bytes(const void *data, size_t size)
        {
            if (!mGood || size > mCapacity - mSize || (size && (!data || !mData)))
            {
                mGood = false;
                return;
            }
            if (size)
                std::memcpy(mData + mSize, data, size);
            mSize += size;
        }
        void u8(uint8_t value) { bytes(&value, 1); }
        void u16(uint16_t value) { u8(uint8_t(value >> 8)); u8(uint8_t(value)); }
        void u32(uint32_t value) { u16(uint16_t(value >> 16)); u16(uint16_t(value)); }
        void u64(uint64_t value) { u32(uint32_t(value >> 32)); u32(uint32_t(value)); }
        void f32(float value)
        {
            static_assert(sizeof(float) == 4 && std::numeric_limits<float>::is_iec559);
            u32(std::bit_cast<uint32_t>(value));
        }
    };

    class WireReader
    {
        const uint8_t *mData;
        size_t mSize, mOffset = 0;
        bool mGood = true;

    public:
        WireReader(const void *data, size_t size): mData(static_cast<const uint8_t *>(data)), mSize(size) {}
        bool good() const { return mGood; }
        bool done() const { return mGood && mOffset == mSize; }
        void bytes(void *data, size_t size)
        {
            if (!mGood || size > mSize - mOffset || (size && (!data || !mData)))
            {
                mGood = false;
                return;
            }
            if (size)
                std::memcpy(data, mData + mOffset, size);
            mOffset += size;
        }
        uint8_t u8() { uint8_t value = 0; bytes(&value, 1); return value; }
        uint16_t u16() { uint16_t high = u8(); return uint16_t((high << 8) | u8()); }
        uint32_t u32() { uint32_t high = u16(); return (high << 16) | u16(); }
        uint64_t u64() { uint64_t high = u32(); return (high << 32) | u32(); }
        float f32() { return std::bit_cast<float>(u32()); }
    };
}
