#include "PlatformContext.hpp"


idk::IPlatformContext *idk::platform::getPlatformContext()
{

}


namespace idk
{
    class IPlatformContext;
    class IPlatformFeature;

    class IAudioBackend;
    class IEventBackend;
    class IFilesystemBackend;
    class IInputBackend;
    class ITimeBackend;
    class IVideoBackend;

    class IPlatformFeature: public idk::Immobile
    {
    private:

    public:
        IPlatformFeature() {  };
        virtual ~IPlatformFeature() = default;
        virtual void update(idk::IPlatformContext&) {  }
    };


    class IPlatformContext: public idk::Immobile
    {
    private:
        std::atomic<bool> mRunning;
        size_t            mNumFeatures;
        IPlatformFeature  *mFeatures[16];

    public:
        IPlatformContext(): mRunning{true}, mNumFeatures(0) {  }

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
            IDK_ASSERT(mNumFeatures < 16, "[idk::IPlatformContext::giveFeature] mFeatures overflow");
            FeatureType *p = idk::New<FeatureType>(args...);
            mFeatures[mNumFeatures++] = p;
            return p;
        }

        // template <typename FeatureType>
        // static idk::IPlatformContext MakeCtx()
        // {
        //     idk::IPlatformContext ctx;
        //     mFeatures.push(new FeatureType());
        //     return ctx;
        // }

        // template <typename FeatureType, typename... Args>
        // static idk::IPlatformContext MakeCtx()
        // {
        //     idk::IPlatformContext ctx = MakeCtx<Args...>();
        //     ctx.mFeatures.push(new FeatureType());
        //     return ctx;
        // }
    };

    namespace platform
    {
        extern idk::IPlatformContext *getPlatformContext();
    }
}
