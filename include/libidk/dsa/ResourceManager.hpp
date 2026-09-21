#pragma once

#include "libidk/Types.hpp"
#include "libidk/StdMem.hpp"

namespace idk
{
    struct ResourceHandle
    {
        int32_t idx;
        int32_t gen;
    };


    template <typename T, int32_t N>
    class ResourceManager: public idk::Immobile
    {
    private:
        alignas(T) uint8_t mObjects[N][sizeof(T)];
        int32_t mGen[N];
        bool    mFree[N];

        ResourceHandle reserveHandle()
        {
            ResourceHandle H { -1, -1 };
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
                mGen[i] = 0;
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
        ResourceHandle createResource(Args&&... args)
        {
            ResourceHandle H = reserveHandle();
            if (H.idx == -1) { return H; }
            new (objPtr(H.idx)) T(std::forward<Args>(args)...);
            return H;
        }

        void destroyResource(ResourceHandle H)
        {
            if (!isAlive(H)) { return; }
            objPtr(H.idx)->~T();
            mGen[H.idx] = static_cast<int32_t>(static_cast<uint32_t>(mGen[H.idx]) + 1U);
            mFree[H.idx] = true;
        }

        bool isAlive(ResourceHandle H)
        {
            int32_t idx = H.idx;
            return (0<=idx && idx<N) && (H.gen == mGen[idx]) && (mFree[idx] == false);
        }

        T *get(ResourceHandle H)
        {
            if (!isAlive(H)) { return nullptr; }
            return objPtr(H.idx);
        }

    };
}