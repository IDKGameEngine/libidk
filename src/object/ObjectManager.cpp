#include "libidk/object/ObjectManager.hpp"
#include "libidk/New.hpp"
#include "libidk/stdmem.hpp"


idk::object::ObjectManager::ObjectManager(int32_t maxObjects)
:   mMaxObjects(maxObjects),
    mGenList(idk::NewArray<int32_t>(maxObjects)),
    mAliveList(idk::NewArray<bool>(maxObjects)),
    mFreelist(idk::NewArray<int32_t>(maxObjects), maxObjects)
{
    idk_memset(mGenList, 0, maxObjects*sizeof(int32_t));
    idk_memset(mAliveList, 0, maxObjects*sizeof(bool));
    idk_memset(mFreelist.data(), 0, maxObjects*sizeof(int32_t));
}


idk::object::Object idk::object::ObjectManager::createObject()
{
    if (mFreelist.empty())
    {
        return {-1, -1};
    }

    int32_t slotIdx = mFreelist.top();
    int32_t genNum  = mGenList[slotIdx];
    mAliveList[slotIdx] = true;

    return {slotIdx, genNum};
}


void idk::object::ObjectManager::destroyObject(Object obj)
{
    if (!bounds_check(obj))
    {
        return;
    }

    int32_t idx = obj.slotIdx;
    if (mAliveList[idx] == false)
    {
        return;
    }
    mFreelist.push(idx);
    mGenList[idx] += 1;
    mAliveList[idx] = false;
}


bool idk::object::ObjectManager::isAlive(Object obj)
{
    if (!bounds_check(obj)) { return false; }
    return mAliveList[obj.slotIdx];
}

