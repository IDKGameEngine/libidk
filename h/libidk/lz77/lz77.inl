#include "lz77.hpp"

namespace idk::lz77
{
    template <size_t ISTREAM_SIZE, size_t OSTREAM_SIZE>
    size_t LZ77Decoder::decode(LZ77InStream<ISTREAM_SIZE>& in, LZ77OutStream<OSTREAM_SIZE>& out)
    {
        LZ77Token token;
        while (in.readNextToken(token))
        {
            size_t start = out.getSize() - token.offset_;
            for (size_t i = 0; i < token.length_; i++)
            {
                out.writeNextByte(out.getByteAtIndex(start + i));
            }
            out.writeNextByte(token.literal_);
        }

        return out.getSize();
    }

    template <typename T, typename U>
    inline static T minValue(const T& x, const U& y)
    {
        return (x < y) ? x : y;
    }

    template <size_t ISTREAM_SIZE, size_t OSTREAM_SIZE>
    size_t LZ77Encoder::encode(LZ77InStream<ISTREAM_SIZE>& in, LZ77OutStream<OSTREAM_SIZE>& out)
    {
        while (in.cursor_ < in.getSize())
        {
            LZ77Match match = findBestMatch(in);
            uint8_t literal = in.getByteAtIndex(in.cursor_ + match.length_);
            out.writeNextToken(LZ77Token(match.offset_, match.length_, literal));
            in.cursor_ += match.length_ + 1;
        }
        return out.getSize();
    }

    template <size_t ISTREAM_SIZE>
    LZ77Encoder::LZ77Match LZ77Encoder::findBestMatch(LZ77InStream<ISTREAM_SIZE>& in)
    {
        size_t keyStart = in.cursor_;
        size_t keyStop = minValue(keyStart + lookaheadSize_, in.getSize() - 1);
        size_t keyLength = keyStop - keyStart;

        size_t searchLength = minValue(searchSize_, in.cursor_);
        size_t searchStart = in.cursor_ - searchLength;
        size_t searchStop = in.cursor_;

        LZ77Match bestMatch(0, 0);
        for (size_t i = searchStart; i < searchStop; i++)
        {
            LZ77Match match = findMatch(in, i, searchStop, keyStart, keyStop);

            // Keep track of best/longest length
            if (match.length_ > bestMatch.length_)
            {
                bestMatch = match;
            }

            // Return early if best length is already key length
            if (bestMatch.length_ == keyLength)
            {
                break;
            }
        }
        return bestMatch;
    }

    template <size_t ISTREAM_SIZE>
    LZ77Encoder::LZ77Match LZ77Encoder::findMatch(LZ77InStream<ISTREAM_SIZE>& in,
                                                    size_t searchStart,
                                                    size_t searchStop,
                                                    size_t keyStart,
                                                    size_t keyStop)
    {
        size_t searchIdx = searchStart;
        size_t keyIdx = keyStart;
        size_t matchLength = 0;

        while ((searchIdx < searchStop) && (keyIdx < keyStop))
        {
            if (in.getByteAtIndex(searchIdx) != in.getByteAtIndex(keyIdx))
            {
                break;
            }

            searchIdx += 1;
            keyIdx += 1;
            matchLength += 1;
        }

        return LZ77Match(keyStart - searchStart, matchLength);
    }

}
