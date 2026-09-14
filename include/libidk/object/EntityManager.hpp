#pragma once

#include "libidk/Types.hpp"
#include "libidk/dsa/Stack.hpp"


namespace idk
{
    struct Entity
    {
        int32_t slotIdx;
        int32_t genNum;
    };

    class EntityManager: public idk::Immobile
    {
    public:
        EntityManager(int32_t maxEntities);
        Entity createEntity();
        void destroyEntity(Entity);
        bool isAlive(Entity);

    private:
        const int32_t       mMaxEntities;
        int32_t            *mGenList;
        bool               *mAliveList;
        idk::Stack<int32_t> mFreelist;

        bool bounds_check(Entity e)
        {
            return (0 <= e.slotIdx) && (e.slotIdx < mMaxEntities);
        }
    };
}

