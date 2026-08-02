#include "libidk/message/LocalRxTx.hpp"
#include "libidk/New.hpp"
#include "libidk/stdmem.hpp"
#include "libidk/assert.hpp"
#include "libidk/log.hpp"

#include <mutex>

static constexpr size_t MAX_REGIONS = 32;
struct LocalRxTxRegion
{
    uint64_t tick;
    uint8_t data[1500];
    LocalRxTxRegion(): tick(0), data() {  }
};
static std::mutex mutexes_[MAX_REGIONS];
static LocalRxTxRegion *regions_[MAX_REGIONS];



idk::LocalRxer::LocalRxer(uint8_t port)
:   mPort(port),
    mTick(0)
{
    std::lock_guard<std::mutex> lock(mutexes_[port]);
    if (regions_[port] == nullptr)
    {
        regions_[port] = idk::New<LocalRxTxRegion>();
    }
}

bool idk::LocalRxer::recvMsg(void *buf, size_t bufsz)
{
    std::lock_guard<std::mutex> lock(mutexes_[mPort]);
    LocalRxTxRegion *r = regions_[mPort];

    if (mTick >= r->tick)
    {
        return false;
    }

    IDK_ASSERT(bufsz <= sizeof(r->data), "bufsz too large!");
    idk_memcpy(buf, r->data, bufsz);
    mTick = r->tick;

    return true;
}



idk::LocalTxer::LocalTxer(uint8_t port)
:   mPort(port)
{
    std::lock_guard<std::mutex> lock(mutexes_[port]);
    if (regions_[port] == nullptr)
    {
        regions_[port] = idk::New<LocalRxTxRegion>();
    }
}

bool idk::LocalTxer::sendMsg(const void *buf, size_t bufsz)
{
    std::lock_guard<std::mutex> lock(mutexes_[mPort]);
    LocalRxTxRegion *r = regions_[mPort];

    IDK_ASSERT(bufsz <= sizeof(r->data), "bufsz too large!");
    idk_memcpy(r->data, buf, bufsz);
    r->tick += 1;

    return true;
}

