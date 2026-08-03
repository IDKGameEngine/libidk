#include "libidk/object/EntityManager.hpp"
#include "libidk/New.hpp"
#include "libidk/StdMem.hpp"


idk::EntityManager::EntityManager(int32_t maxEntities)
:   mMaxEntities(maxEntities),
    mGenList(idk::NewArray<int32_t>(maxEntities)),
    mAliveList(idk::NewArray<bool>(maxEntities)),
    mFreelist(idk::NewArray<int32_t>(maxEntities), maxEntities)
{
    idk_memset(mGenList, 0, maxEntities*sizeof(int32_t));
    idk_memset(mAliveList, 0, maxEntities*sizeof(bool));
    idk_memset(mFreelist.data(), 0, maxEntities*sizeof(int32_t));
}


idk::Entity idk::EntityManager::createEntity()
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


void idk::EntityManager::destroyEntity(Entity obj)
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


bool idk::EntityManager::isAlive(Entity obj)
{
    if (!bounds_check(obj)) { return false; }
    return mAliveList[obj.slotIdx];
}

