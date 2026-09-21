#pragma once

#include "libidk/Types.hpp"
#include "libidk/StdMem.hpp"

namespace idk
{
    struct ObjectHandle
    {
        int32_t idx;
        int32_t gen;
    };


    template <typename DataType, int32_t N>
    class ObjectManager: public idk::Immobile
    {
    private:
        alignas(DataType) uint8_t mObjects[N][sizeof(DataType)];
        int32_t mGen[N];
        bool    mFree[N];

        ObjectHandle reserveHandle()
        {
            ObjectHandle H { -1, -1 };
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

        DataType *objPtr(int32_t idx)
        {
            return reinterpret_cast<DataType*>(mObjects[idx]);
        }

    public:
        ObjectManager()
        {
            for (int32_t i=0; i<N; i++)
            {
                mGen[i] = 0;
                mFree[i] = true;
            }
        }

        ~ObjectManager()
        {
            for (int32_t i=0; i<N; i++)
            {
                if (mFree[i] == false)
                {
                    objPtr(i)->~DataType();
                }
            }
        }

        template <typename... Args>
        ObjectHandle createObject(Args&&... args)
        {
            ObjectHandle H = reserveHandle();
            if (H.idx == -1) { return H; }
            new (objPtr(H.idx)) DataType(std::forward<Args>(args)...);
            return H;
        }

        void destroyObject(ObjectHandle H)
        {
            if (!isAlive(H)) { return; }
            objPtr(H.idx)->~DataType();
            mGen[H.idx] = static_cast<int32_t>(static_cast<uint32_t>(mGen[H.idx]) + 1U);
            mFree[H.idx] = true;
        }

        bool isAlive(ObjectHandle H)
        {
            int32_t idx = H.idx;
            return (0<=idx && idx<N) && (H.gen == mGen[idx]) && (mFree[idx] == false);
        }

        DataType *get(ObjectHandle H)
        {
            if (!isAlive(H)) { return nullptr; }
            return objPtr(H.idx);
        }

    };
}