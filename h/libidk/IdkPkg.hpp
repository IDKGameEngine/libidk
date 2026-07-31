#pragma once

#include "libidk/types.hpp"
#include "libidk/assert.hpp"
#include "libidk/stdstr.hpp"
#include "libidk/stdmem.hpp"
#include "libidk/Memory.hpp"
#include "libidk/Tuple.hpp"

namespace idk::core
{
    struct PkgEntry
    {
        static constexpr uint64_t MAGIC = 0xDEADBEBE;
        static constexpr uint64_t MAX_NAME_LENGTH = 240;

        uint64_t magic_;
        uint64_t size_;
        char name_[MAX_NAME_LENGTH];

        PkgEntry(uint64_t size=0, const char *name="")
        :   magic_(MAGIC),
            size_(size)
        {
            idk_strncpy(name_, name, MAX_NAME_LENGTH-1);
            name_[MAX_NAME_LENGTH - 1] = '\0';
        }
    };

    template <size_t MAX_SIZE>
    class PkgWriter : public idk::NonCopyable, public idk::NonMovable
    {
    private:
        uint8_t mData[MAX_SIZE];
        uint8_t *mTail;
        uint8_t *mEnd;

        void writeBytes(const void *src, size_t size)
        {
            idk_memcpy(mTail, src, size);
            mTail += size;
        }

    public:
        PkgWriter(): mTail(mData), mEnd(mData + MAX_SIZE) {  }

        bool writeFile(const char *filename, const void *data, size_t size)
        {
            if (mTail + sizeof(PkgEntry) + size >= mEnd)
            {
                return false;
            }

            PkgEntry entry(size, filename);
            writeBytes(&entry, sizeof(entry));
            writeBytes(data, size);
        
            return true;
        }

        const void *getData() const { return mData; }
        size_t getSize() const { return mTail - mData; }

    };

    class PkgReader
    {
    private:
        const uint8_t *mBase;
        const uint8_t *mEnd;

        const uint8_t *findFile(const char *name)
        {
            const uint8_t *addr = mBase;
            while (addr < mEnd)
            {
                PkgEntry e = idk::core::unalignedLoad<PkgEntry>(addr);
                if (idk_strncmp(e.name_, name, PkgEntry::MAX_NAME_LENGTH) == 0)
                {
                    return addr;
                }
                addr += sizeof(PkgEntry) + e.size_;
            }
            return nullptr;
        }

    public:
        struct FileEntry
        {
            const char *name;
            const char *data;
            const size_t size;
        };

        PkgReader(const void *srcBuf, size_t srcSize)
        : mBase((const uint8_t*)srcBuf), mEnd(mBase + srcSize) {  }

        FileEntry readFile(const char *name)
        {
            const uint8_t *addr = findFile(name);
            if (addr == nullptr)
            {
                return FileEntry{"???", nullptr, 0};
            }
            PkgEntry e = idk::core::unalignedLoad<PkgEntry>(addr);
            IDK_ASSERT(e.magic_ == PkgEntry::MAGIC, "Invalid PkgEntry read");
            return FileEntry{e.name_, (const char*)addr + sizeof(PkgEntry), e.size_};
        }

        struct Iterator
        {
            const uint8_t *mAddr;
            Iterator(const uint8_t *addr): mAddr(addr) {  }
            Iterator(const Iterator &I): mAddr(I.mAddr) {  }

            Iterator &operator++()
            {
                PkgEntry e = idk::core::unalignedLoad<PkgEntry>(mAddr);
                mAddr += sizeof(PkgEntry) + e.size_;
                return *this;
            }

            Iterator operator++(int)
            {
                PkgEntry e = idk::core::unalignedLoad<PkgEntry>(mAddr);
                return Iterator(mAddr + sizeof(PkgEntry) + e.size_);
            }

            bool operator==(const Iterator &rhs) { return mAddr == rhs.mAddr; }
            bool operator!=(const Iterator &rhs) { return mAddr != rhs.mAddr; }

            FileEntry operator*()
            {
                // PkgEntry e = idk::core::unalignedLoad<PkgEntry>(mAddr);
                const PkgEntry *e = (const PkgEntry*)mAddr;
                return FileEntry{e->name_, (const char*)mAddr + sizeof(PkgEntry), e->size_};
            };
        };

        Iterator begin() { return Iterator(mBase); }
        Iterator end()   { return Iterator(mEnd); }
    };

}

