#pragma once

#include "libidk/Types.hpp"
#include "libidk/Stack.hpp"


namespace idk::object
{
    struct Object
    {
        int32_t slotIdx;
        int32_t genNum;
    };

    class ObjectManager: public idk::Immobile
    {
    public:
        ObjectManager(int32_t maxObjects);
        Object createObject();
        void destroyObject(Object);
        bool isAlive(Object);

    private:
        // static constexpr int32_t MAX_OBJECTS = 1024;
        const int32_t       mMaxObjects;
        int32_t            *mGenList;
        bool               *mAliveList;
        idk::Stack<int32_t> mFreelist;

        bool bounds_check(Object obj)
        {
            return (0 <= obj.slotIdx) && (obj.slotIdx < mMaxObjects);
        }
    };
}

