#pragma once

#include "libidk/Types.hpp"


namespace idk
{
    class SharedMemory: public idk::Immobile
    {
    public:
        static constexpr size_t MAX_NAME_LENGTH = 64;

    private:
        int mFd = -1;
        size_t mSize = 0;
        bool mHasOwnership = false;
        void *mBase = nullptr;
        char mName[MAX_NAME_LENGTH];

    public:
        static void *create(const char *name, size_t size);
        static void *open(const char *name, size_t size);

        // SharedMemory(): mFd(-1), mSize(0), mHasOwnership(false), mBase(nullptr) {  }
        // SharedMemory(int fd, size_t size, const char *name, bool hasOwnership);
        // ~SharedMemory();
        // SharedMemory(const SharedMemory &) = delete;
        // SharedMemory(SharedMemory &&) = default;

        // void *data() const { return mBase; }
        // size_t size() const { return mSize; }

    };

}
