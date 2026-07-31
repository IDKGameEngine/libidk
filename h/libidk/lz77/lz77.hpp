#pragma once

#include "libidk/Types.hpp"
#include <cstring>

namespace idk::lz77
{
    struct __attribute__((packed)) LZ77Token
    {
        uint16_t offset_;
        uint8_t length_;
        uint8_t literal_;

        LZ77Token(){};
        LZ77Token(uint16_t offset, uint8_t length, uint8_t literal)
        : offset_(offset), length_(length), literal_(literal) {  }
    };


    template <size_t MAX_SIZE>
    class LZ77InStream
    {
    private:
        const uint8_t (&data_)[MAX_SIZE];
        const size_t size_;

    public:
        size_t cursor_;

        LZ77InStream(const uint8_t (&data)[MAX_SIZE], size_t size)
        : data_(data), size_(size), cursor_(0) {  }

        template <typename T>
        bool readNext(T& data)
        {
            if (cursor_ + sizeof(T) > size_)
            {
                return false;
            }
            std::memcpy(&data, &data_[cursor_], sizeof(T));
            cursor_ += sizeof(T);
            return true;
        }

        uint8_t getByteAtIndex(size_t idx) const
        {
            if (idx >= size_)
            {
                return 0;
            }
            return data_[idx];
        }

        inline size_t getSize() const { return size_; }
    };


    template <size_t MAX_SIZE>
    class LZ77OutStream
    {
    private:
        uint8_t (&data_)[MAX_SIZE];
        size_t size_;

    public:
        LZ77OutStream(uint8_t (&data)[MAX_SIZE]) : data_(data), size_(0) {}

        template <typename T>
        bool writeNext(const T& data)
        {
            if (size_ + sizeof(T) > MAX_SIZE)
            {
                return false;
            }
            std::memcpy(&data_[size_], &data, sizeof(T));
            size_ += sizeof(T);
            return true;
        }

        uint8_t getByteAtIndex(size_t idx) const
        {
            if (idx >= size_)
            {
                return 0;
            }
            return data_[idx];
        }

        inline size_t getSize() const { return size_; }

    };


    class LZ77Decoder
    {
    public:
        /// @return Size of output stream
        template <size_t ISTREAM_SIZE, size_t OSTREAM_SIZE>
        size_t decode(LZ77InStream<ISTREAM_SIZE>& in, LZ77OutStream<OSTREAM_SIZE>& out);
    };


    class LZ77Encoder
    {
    public:
        LZ77Encoder(uint16_t searchSize = 1024, uint8_t lookaheadSize = 64)
            : searchSize_((searchSize)), lookaheadSize_(lookaheadSize){};

        /// @return Size of output stream
        template <size_t ISTREAM_SIZE, size_t OSTREAM_SIZE>
        size_t encode(LZ77InStream<ISTREAM_SIZE>& in, LZ77OutStream<OSTREAM_SIZE>& out);

    private:
        const uint16_t searchSize_;
        const uint8_t lookaheadSize_;

        struct LZ77Match
        {
            uint16_t offset_;
            uint8_t length_;
            LZ77Match(uint16_t offset, uint8_t length) : offset_(offset), length_(length){};
            LZ77Match() : offset_(0), length_(0){};
        };

        template <size_t ISTREAM_SIZE>
        LZ77Match findBestMatch(LZ77InStream<ISTREAM_SIZE>& in);

        template <size_t ISTREAM_SIZE>
        LZ77Match findMatch(LZ77InStream<ISTREAM_SIZE>& in,
                            size_t searchStart,
                            size_t searchStop,
                            size_t keyStart,
                            size_t keyStop);
    };

}

#include "lz77.inl"
