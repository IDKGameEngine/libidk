#pragma once

#include "libidk/Types.hpp"
#include <tuple>
#include <new>


namespace idk
{
    template <typename T>
    struct ResourceHandle
    {
        static constexpr ResourceHandle Invalid { NumericMax<uint32_t>, NumericMax<uint32_t> };

        uint32_t idx;
        uint32_t gen;

        auto operator<=>(const ResourceHandle&) const = default;
    };


    template <typename T, uint32_t N>
    class ResourceManager: public idk::Immobile
    {
    public:
        static_assert(N > 0);
        using Handle = ResourceHandle<T>;

        ResourceManager();
        ~ResourceManager();

        Handle createResource(T&&);
        void   destroyResource(Handle H);
        bool   isAlive(Handle H) const;

        inline       T *get(Handle H)       { return isAlive(H) ? objptr(H.idx) : nullptr; };
        inline const T *get(Handle H) const { return isAlive(H) ? objptr(H.idx) : nullptr; };

        class Iterator
        {
        private:
            ResourceManager *mOwner;
            uint32_t mIdx;

            void skipFree()
            {
                while (mIdx < N && mOwner->mOccupied[mIdx] == false)
                {
                    mIdx++;
                }
            }

        public:
            using Entry = std::tuple<Handle, T&>;
            Iterator(ResourceManager *o, uint32_t i): mOwner(o), mIdx(i) { skipFree(); }
            Entry operator*() const { return Entry{{mIdx, mOwner->mGen[mIdx]}, *mOwner->objptr(mIdx)}; }
            Iterator &operator++() { mIdx++; skipFree(); return *this; }
            bool operator==(const Iterator&) const = default;

        };

        Iterator begin() { return Iterator(this, 0); }
        Iterator end()   { return Iterator(this, N); }


    private:
        struct RawData { alignas(T) uint8_t data[sizeof(T)]; };

        RawData  mData[N];
        uint32_t mGen[N];
        bool     mOccupied[N];

        inline       T *objptr(uint32_t i);
        inline const T *objptr(uint32_t i) const;
    
        Handle reserveHandle();
    };
}

#include "ResourceManager.inl"
