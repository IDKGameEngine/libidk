#include "libidk/message/LocalRxTx.hpp"
#include "libidk/New.hpp"
#include "libidk/stdmem.hpp"
#include "libidk/assert.hpp"
#include "libidk/log.hpp"

#include <mutex>

static constexpr size_t MAX_REGIONS = 32;
struct LocalRxTxRegion
{
    std::mutex mutex;
    int64_t tick;
    uint8_t data[1500];
    LocalRxTxRegion(): mutex(), tick(0), data() {  }
};
static LocalRxTxRegion *localRegions[MAX_REGIONS];


idk::LocalRxer::LocalRxer(uint8_t port)
:   mRegion(nullptr),
    mTick(0)
{
    if (localRegions[port] == nullptr)
    {
        localRegions[port] = idk::New<LocalRxTxRegion>();
    }
    mRegion = localRegions[port];
}

bool idk::LocalRxer::recvMsg(void *buf, size_t bufsz)
{
    auto *r = (LocalRxTxRegion*)mRegion;
    std::lock_guard<std::mutex>(r->mutex);
    if (mTick < r->tick)
    {
        mTick = r->tick;
        IDK_ASSERT(bufsz <= sizeof(r->data), "bufsz too large!");
        idk_memcpy(buf, r->data, bufsz);
        return true;
    }
    return false;
}



idk::LocalTxer::LocalTxer(uint8_t port)
:   mRegion(nullptr)
{
    if (localRegions[port] == nullptr)
    {
        localRegions[port] = idk::New<LocalRxTxRegion>();
    }
    mRegion = localRegions[port];
}

bool idk::LocalTxer::sendMsg(const void *buf, size_t bufsz)
{
    auto *r = (LocalRxTxRegion*)mRegion;
    std::lock_guard<std::mutex>(r->mutex);
    IDK_ASSERT(bufsz <= sizeof(r->data), "bufsz too large!");
    idk_memcpy(r->data, buf, bufsz);
    r->tick += 1;
    return true;
}

