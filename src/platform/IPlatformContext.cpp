#include "libidk/platform/IPlatformContext.hpp"
#include "libidk/platform/IPlatformFeature.hpp"
#include "libidk/log.hpp"


idk::IPlatformContext::IPlatformContext()
:   mRunning{true},
    mShutdown{false},
    mFeatureIdx(0)
{

}

bool idk::IPlatformContext::running()
{
    return mRunning.load(std::memory_order_acquire);
}

void idk::IPlatformContext::shutdown()
{
    mShutdown.store(true, std::memory_order_release);
}

void idk::IPlatformContext::update()
{
    update_features();

    if (mShutdown.load(std::memory_order_acquire))
    {
        kill_features();
        mRunning.store(false, std::memory_order_release);
    }
}

void idk::IPlatformContext::update_features()
{
    for (int32_t i=0; i<mFeatureIdx; i++)
    {
        mFeatures[i]->update(this);
    }
}

void idk::IPlatformContext::kill_features()
{
    for (int32_t i=0; i<mFeatureIdx; i++)
    {
        mFeatures[i]->~IPlatformFeature();
    }
}
