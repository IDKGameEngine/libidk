#pragma once

#include "libidk/Types.hpp"
#include "libidk/dsa/List.hpp"
#include "libidk/New.hpp"
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
        size_t            mNumFeatures;
        IPlatformFeature *mFeatures[16];
        // idk::InplaceList<IPlatformFeature*, 16> mFeatures;

    public:
        PlatformContext()
        :   mRunning{true}, mNumFeatures(0)
        { 

        }

        bool running()  { return mRunning.load(); }
        void shutdown() { mRunning.store(false); }

        void update()
        {
            for (size_t i=0; i<mNumFeatures; i++)
            {
                mFeatures[i]->update(*this);
            }
        }


        template <typename FeatureType, typename... Args>
        FeatureType *giveFeature(Args&&... args)
        {
            IDK_ASSERT(mNumFeatures < 16, "[PlatformContext::giveFeature] mFeatures overflow");
            FeatureType *p = idk::New<FeatureType>(args...);
            mFeatures[mNumFeatures++] = p;
            return p;
        }

        // template <typename FeatureType>
        // static PlatformContext MakeCtx()
        // {
        //     PlatformContext ctx;
        //     mFeatures.push(new FeatureType());
        //     return ctx;
        // }

        // template <typename FeatureType, typename... Args>
        // static PlatformContext MakeCtx()
        // {
        //     PlatformContext ctx = MakeCtx<Args...>();
        //     ctx.mFeatures.push(new FeatureType());
        //     return ctx;
        // }
    };

}
