#pragma once

#include "libidk/Types.hpp"
#include <mutex>

namespace idk
{
    template <typename T>
    struct LocalPortMemoryRegion
    {
        std::mutex mutex;
        int64_t tick;
        T data;
        LocalPortMemoryRegion() : mutex(), tick(0), data() {  };
    };


    template <typename T>
    class LocalPortWriter: private idk::Immobile
    {
    private:
        LocalPortMemoryRegion<T> &mRegion;

    public:
        LocalPortWriter(LocalPortMemoryRegion<T> &region)
        :   mRegion(region)
        {

        }

        void writeData(const T &data)
        {
            std::lock_guard<std::mutex> lock(mRegion.mutex);
            mRegion.tick += 1;
            mRegion.data = data;
        }
    };


    template <typename T>
    class LocalPortReader: private idk::Immobile
    {
    private:
        LocalPortMemoryRegion<T> &mRegion;
        int64_t mTick;

    public:
        LocalPortReader(const LocalPortMemoryRegion<T> &region)
        :   mRegion(region),
            mTick(0)
        {

        }

        bool readData(T &data)
        {
            std::lock_guard<std::mutex> lock(mRegion.mutex);

            if (mTick >= mRegion.tick)
            {
                return false;
            }

            mTick = mRegion.tick;
            data = mRegion.data;
            return true;
        }
    };
}

