#pragma once

#include "libidk/Types.hpp"
#include "libidk/dsa/List.hpp"
#include <atomic>


namespace idk
{
    class IPlatformFeature;
    class PlatformContext;


    class IPlatformFeature: public idk::Immobile
    {
    private:

    public:
        IPlatformFeature() {  };
        virtual ~IPlatformFeature() = default;
        virtual void update(PlatformContext&) {  }
    };


    class PlatformContext: public idk::Immobile
    {
    private:
        std::atomic<bool> mRunning;
        idk::InplaceList<IPlatformFeature*, 16> mFeatures;
        
    public:
        PlatformContext(): mRunning{true} {  }

        bool running()  { return mRunning.load(); }
        void shutdown() { mRunning.store(false); }

        void update()
        {
            for (auto *feature: mFeatures)
            {
                feature->update(*this);
            }
        }


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
