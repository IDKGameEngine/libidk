#include "libidk/message/LocalRxTx.hpp"
#include "libidk/New.hpp"
#include "libidk/Serialize.hpp"
#include "libidk/StdMem.hpp"
#include "libidk/Assert.hpp"
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
    mTick(0),
    mHeader(mMsgBuf.header),
    mPayload(mMsgBuf.payload),
    mMsgBuf(),
    mRecvInfo()
{
    std::lock_guard<std::mutex> lock(mutexes_[port]);
    if (regions_[port] == nullptr)
    {
        regions_[port] = idk::New<LocalRxTxRegion>();
    }
}


idk::MessageRecvInfo *idk::LocalRxer::recvMsg()
{
    std::lock_guard<std::mutex> lock(mutexes_[mPort]);
    LocalRxTxRegion *r = regions_[mPort];

    if (mTick >= r->tick)
    {
        return nullptr;
    }
    mTick = r->tick;

    ByteDecoder decoder(r->data, sizeof(r->data));
    decoder.read(mHeader);
    decoder.read(mPayload.data, mHeader.payloadSize);

    mRecvInfo.header = mHeader;
    mRecvInfo.payload = mPayload.data;

    return &mRecvInfo;
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

bool idk::LocalTxer::sendMsg(const void *srcBuf, size_t srcSize, MessagePayloadType payloadType)
{
    std::lock_guard<std::mutex> lock(mutexes_[mPort]);

    LocalRxTxRegion *r = regions_[mPort];
    ByteEncoder encoder(r->data, sizeof(r->data));
    encoder.write(MessageHeader(0, srcSize, payloadType));
    if (!encoder.write(srcBuf, srcSize))
    {
        VLOG_WARN("[LocalTxer::sendMsg] Failed to send message: srcSize too large");
        return false;
    }
    r->tick += 1;

    return true;
}

