#include "libidk/object/AppObject.hpp"


idk::AppObject::AppObject(const char *subClassName)
:   mLibraryId(LibIdkClassID),
    mBaseClassId(AppObjectClassID),
    mSubClassId(subClassName)
{

}


bool idk::AppObject::isAppObject(const void *ptr)
{
    const char *tail = (const char*)ptr;

    auto &libIdkId = LibIdkClassID;
    if (0 != idk_strncmp(tail, &libIdkId[0], libIdkId.getStrLen()))
        return false;
    tail += libIdkId.getMemLen();

    auto &appObjId = AppObjectClassID;
    if (0 != idk_strncmp(tail, &appObjId[0], appObjId.getStrLen()))
        return false;
    tail += appObjId.getMemLen();

    return true;
}

