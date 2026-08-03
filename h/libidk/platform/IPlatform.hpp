#pragma once

#include "libidk/IdkObject.hpp"

namespace idk
{
    struct IPlatformObject: public IdkObject
    {
        IPlatformObject(const char *typeMeta): IdkObject(typeMeta) {}
    };

    class IPlatformAudio;
    class IPlatformFileIo;
    class IPlatformUserIo;
    class IPlatformVideo;
}
