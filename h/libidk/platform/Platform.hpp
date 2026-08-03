#pragma once

#include "libidk/Types.hpp"
#include "libidk/List.hpp"

namespace idk
{
    class IPlatformFeature;

    class PlatformContext: public idk::Immobile
    {
    private:
        idk::InplaceList<IPlatformFeature*, 16> mFeatures;
        
    public:
        void update();

        template <typename FeatureType>
        static PlatformContext BuildPlatformContext()
        {
            PlatformContext ctx;
            mFeatures.push(new FeatureType());
            return ctx;
        }

        template <typename FeatureType, typename... Args>
        static PlatformContext BuildPlatformContext()
        {
            PlatformContext ctx = BuildPlatformContext<Args...>();
            ctx.mFeatures.push(new FeatureType());
            return ctx;
        }
    };

}
