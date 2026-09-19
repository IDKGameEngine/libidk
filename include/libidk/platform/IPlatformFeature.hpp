#pragma once

#include "libidk/Types.hpp"


namespace idk
{
    class IPlatformContext;

    class IPlatformFeature: public idk::Immobile
    {
    public:
        virtual ~IPlatformFeature() = default;
        virtual void update(IPlatformContext*) {  };
    };
}
