#pragma once

#include "libidk/Types.hpp"
#include "libidk/object/ObjectClassID.hpp"

namespace idk
{
    struct AppObject: public idk::Immobile
    {
        static inline const ObjectClassID<6>  LibIdkClassID    = ObjectClassID<6>("idk::");
        static inline const ObjectClassID<32> AppObjectClassID = ObjectClassID<32>("AppObject");

        const ObjectClassID<6>  mLibraryId;
        const ObjectClassID<32> mBaseClassId;
        const ObjectClassID<32> mSubClassId;

        AppObject(const char *subClassName);
        static bool isAppObject(const void *ptr);
    };

    #define IDK_APPOBJECT_CTOR(DerivedType) DerivedType(): idk::AppObject(#DerivedType) {  }


    class TestClass: public idk::AppObject
    {
    public:
        IDK_APPOBJECT_CTOR(TestClass);

    };
}
