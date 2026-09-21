#pragma once

#include "libidk/Types.hpp"
#include "libidk/StdMem.hpp"

namespace idk
{
    template <typename T>
    struct ResourceHandle
    {
        int32_t idx;
        int32_t gen;
    };


    template <typename T, int32_t N>
    class ResourceManager: public idk::Immobile
    {
    public:
        using Handle = ResourceHandle<T>;

    private:
        alignas(T) uint8_t mObjects[N][sizeof(T)];
        int32_t mGen[N];
        bool    mFree[N];

        Handle reserveHandle()
        {
            Handle H { -1, -1 };
            for (int32_t i=0; i<N; i++)
            {
                if (mFree[i] == true)
                {
                    H = { i, mGen[i] };
                    mFree[i] = false;
                    break;
                }
            }
            return H;
        }

        T *objPtr(int32_t idx)
        {
            return reinterpret_cast<T*>(mObjects[idx]);
        }

    public:
        ResourceManager()
        {
            for (int32_t i=0; i<N; i++)
            {
                mGen[i] = 1;
                mFree[i] = true;
            }
        }

        ~ResourceManager()
        {
            for (int32_t i=0; i<N; i++)
            {
                if (mFree[i] == false)
                {
                    objPtr(i)->~T();
                }
            }
        }

        template <typename... Args>
        Handle createResource(Args&&... args)
        {
            Handle H = reserveHandle();
            if (H.idx == -1) { return H; }
            new (objPtr(H.idx)) T(std::forward<Args>(args)...);
            return H;
        }

        void destroyResource(Handle H)
        {
            if (!isAlive(H)) { return; }
            objPtr(H.idx)->~T();
            mGen[H.idx] = static_cast<int32_t>(static_cast<uint32_t>(mGen[H.idx]) + 1U);
            mFree[H.idx] = true;
        }

        bool isAlive(Handle H)
        {
            int32_t idx = H.idx;
            return (0<=idx && idx<N) && (H.gen == mGen[idx]) && (mFree[idx] == false);
        }

        T *get(Handle H)
        {
            if (!isAlive(H)) { return nullptr; }
            return objPtr(H.idx);
        }


        class Iterator
        {
        public:
            using Entry = std::tuple<Handle, T&>;

            Iterator(ResourceManager *owner, int32_t idx): mOwner(owner), mIdx(idx)
            {
                skipFree();
            }

            Entry operator*() const
            {
                return Entry{ Handle{mIdx, mOwner->mGen[mIdx]}, *mOwner->objPtr(mIdx) };
            }

            Iterator &operator++()
            {
                mIdx++;
                skipFree();
                return *this;
            }

            bool operator!=(const Iterator &other) const { return mIdx != other.mIdx; }
            bool operator==(const Iterator &other) const { return mIdx == other.mIdx; }

        private:
            void skipFree()
            {
                while (mIdx < N && mOwner->mFree[mIdx] == true)
                {
                    mIdx++;
                }
            }

            ResourceManager *mOwner;
            int32_t mIdx;
        };

        Iterator begin() { return Iterator(this, 0); }
        Iterator end()   { return Iterator(this, N); }

    };
}