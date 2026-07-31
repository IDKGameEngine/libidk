#pragma once

#include "libidk/Types.hpp"

namespace idk
{
    template <typename T>
    class LocalPortWriter: private idk::Immobile
    {
    private:
        T &mDst;

    public:
        LocalPortWriter(T &dst);
        bool writeData(const T &data) {  mDst = data; }

    };

    template <typename T>
    class LocalPortReader: private idk::Immobile
    {
    private:
        const T &mSrc;

    public:
        LocalPortReader(const T &src);
        const T &readData() { return mSrc; }

    };
}

