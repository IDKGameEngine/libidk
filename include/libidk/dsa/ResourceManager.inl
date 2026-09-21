#pragma once

template <typename T, uint32_t N>
idk::ResourceManager<T, N>::ResourceManager()
{
    for (uint32_t i=0; i<N; i++)
    {
        mGen[i] = 0;
        mOccupied[i] = false;
    }
}

template <typename T, uint32_t N>
idk::ResourceManager<T, N>::~ResourceManager()
{
    for (uint32_t i=0; i<N; i++)
    {
        if (mOccupied[i])
        {
            std::destroy_at(objptr(i));
        }
    }
}


// template <typename T, uint32_t N>
// idk::ResourceManager<T, N>::Handle idk::ResourceManager<T, N>::createResource(const T &value)
// {
//     Handle H = reserveHandle();
//     if (H != Handle::Invalid)
//     {
//         new (objptr(H.idx)) T(value);
//     }
//     return H;
// }

template <typename T, uint32_t N>
idk::ResourceManager<T, N>::Handle idk::ResourceManager<T, N>::createResource(T &&value)
{
    Handle H = reserveHandle();
    if (H != Handle::Invalid)
    {
        new (objptr(H.idx)) T(std::move(value));
    }
    return H;
}


template <typename T, uint32_t N>
void idk::ResourceManager<T, N>::destroyResource(Handle H)
{
    if (isAlive(H))
    {
        objptr(H.idx)->~T();
        mGen[H.idx] += 1;
        mOccupied[H.idx] = false;
    }
}


template <typename T, uint32_t N>
bool idk::ResourceManager<T, N>::isAlive(Handle H) const
{
    return (H.idx < N) && (H.gen == mGen[H.idx]) && (mOccupied[H.idx]);
}


template <typename T, uint32_t N>
T *idk::ResourceManager<T, N>::objptr(uint32_t i)
{
    return std::launder(reinterpret_cast<T*>(mData[i].data));
}

template <typename T, uint32_t N>
const T* idk::ResourceManager<T, N>::objptr(uint32_t i) const
{
    return std::launder(reinterpret_cast<const T*>(mData[i].data));
}


template <typename T, uint32_t N>
idk::ResourceManager<T, N>::Handle idk::ResourceManager<T, N>::reserveHandle()
{
    for (uint32_t i=0; i<N; i++)
    {
        if (mOccupied[i] == false)
        {
            mOccupied[i] = true;
            return Handle{ i, mGen[i] };
        }
    }
    return Handle::Invalid;
}
