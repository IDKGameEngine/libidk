#include "libidk/platform/IPlatformContext.hpp"
#include "libidk/platform/IPlatformFeature.hpp"


idk::IPlatformContext::IPlatformContext()
:   mRunning{true},
    mFeatureIdx(0)
{

}

bool idk::IPlatformContext::running() const noexcept
{
    return mRunning.load(std::memory_order_acquire);
}

void idk::IPlatformContext::shutdown() noexcept
{
    mRunning.store(false, std::memory_order_release);
}

void idk::IPlatformContext::update()
{
    for (int32_t i=0; i<mFeatureIdx; i++)
    {
        mFeatures[i]->update(this);
    }
}
