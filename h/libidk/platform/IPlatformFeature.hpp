#pragma once

#include "libidk/IdkObject.hpp"


namespace idk
{
    class PlatformContext;

    class IPlatformFeature: public IdkObject
    {
    private:

    public:
        using IdkObject::IdkObject;
        virtual void update(PlatformContext&) = 0;
    };
}
