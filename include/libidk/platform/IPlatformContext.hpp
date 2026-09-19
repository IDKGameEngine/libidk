#pragma once

#include "libidk/Types.hpp"
#include "libidk/Assert.hpp"
#include "libidk/New.hpp"
#include <atomic>


namespace idk
{
    class IPlatformFeature;

    class IPlatformContext: public idk::Immobile
    {
    public:
        IPlatformContext();
        virtual ~IPlatformContext() = default;

        bool running();
        void shutdown();
        void update();

        template <typename FeatureType, typename... Args>
        FeatureType &addFeature(Args&&... args)
        {
            IDK_ASSERT(mFeatureIdx<16, "[idk::IPlatformContext::giveFeature] mFeatures overflow");
            mFeatures[mFeatureIdx++] = idk::New<FeatureType>(args...);
            return *dynamic_cast<FeatureType*>(mFeatures[mFeatureIdx-1]);
        }

        template <typename FeatureType>
        FeatureType *getFeature() noexcept
        {
            for (int32_t i=0; i<mFeatureIdx; i++)
            {
                if (FeatureType *ft = dynamic_cast<FeatureType*>(mFeatures[i]))
                {
                    return ft;
                }
            }
            return nullptr;
        }

    private:
        std::atomic<bool>  mRunning {true};
        std::atomic<bool>  mShutdown {false};
        int32_t            mFeatureIdx;
        IPlatformFeature  *mFeatures[16];

        void update_features();
        void kill_features();

    };

}
