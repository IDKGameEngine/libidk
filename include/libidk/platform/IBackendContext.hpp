#pragma once

#include "libidk/Types.hpp"
#include "libidk/Assert.hpp"
#include "libidk/New.hpp"
#include <atomic>


namespace idk
{
    class BackendContext;
    class IBackendFeature;

    class IAudioBackend;
    class IEventBackend;
    class IFilesystemBackend;
    class IInputBackend;
    class ITimeBackend;
    class IVideoBackend;

    class IBackendFeature: public idk::Immobile
    {
    private:

    public:
        IBackendFeature() {  };
        virtual ~IBackendFeature() = default;
        virtual void update(idk::BackendContext&) {  }
    };


    class BackendContext: public idk::Immobile
    {
    private:
        std::atomic<bool> mRunning;
        size_t            mNumFeatures;
        IBackendFeature  *mFeatures[16];

    public:
        BackendContext(): mRunning{true}, mNumFeatures(0) {  }

        bool running()
        {
            return mRunning.load();
        }

        void shutdown()
        {
            mRunning.store(false);
        }

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
            IDK_ASSERT(mNumFeatures < 16, "[idk::BackendContext::giveFeature] mFeatures overflow");
            FeatureType *p = idk::New<FeatureType>(args...);
            mFeatures[mNumFeatures++] = p;
            return p;
        }

        // template <typename FeatureType>
        // static idk::BackendContext MakeCtx()
        // {
        //     idk::BackendContext ctx;
        //     mFeatures.push(new FeatureType());
        //     return ctx;
        // }

        // template <typename FeatureType, typename... Args>
        // static idk::BackendContext MakeCtx()
        // {
        //     idk::BackendContext ctx = MakeCtx<Args...>();
        //     ctx.mFeatures.push(new FeatureType());
        //     return ctx;
        // }
    };
}
