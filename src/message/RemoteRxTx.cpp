#include <SDL3_net/SDL_net.h>

#include "libidk/message/RemoteRxTx.hpp"
#include "libidk/Serialize.hpp"

#include "libidk/stdmem.hpp"
#include "libidk/stdstr.hpp"
#include "libidk/math.hpp"
#include "libidk/log.hpp"



idk::RemoteRxer::RemoteRxer(uint16_t port)
:   mAuthToken(0xDEADBEBE),
    mSocket(nullptr),
    mLastDatagram(nullptr),
    mPort(port)
{
    if (!NET_Init())
    {
        VLOG_FATAL("[RemoteRxer::RemoteRxer] Failure initializing SDL3_Net: {}", SDL_GetError());
    }

    if (!(mSocket = NET_CreateDatagramSocket(NULL, mPort, 0)))
    {
        VLOG_FATAL("[RemoteRxer::RemoteRxer] Failure creating socket: {}", SDL_GetError());
    }
}


bool idk::RemoteRxer::recvMsg(void *dstBuf, size_t dstSize)
{
    NET_Datagram *d = beginRecvMsg();
    if (d == nullptr) { return false; }

    ByteDecoder decoder(d->buf, d->buflen);
    auto header = decoder.read<RemoteMessageHeader>();
    if (header.authToken != mAuthToken)
    {
        VLOG_WARN("[RemoteRxer::recvMsg] header.authToken != mAuthToken");
        return badRecvMsg(d);
    }
    if (header.payloadSize > dstSize)
    {
        VLOG_WARN("[RemoteRxer::recvMsg] payloadSize is larger than dstSize");
        return badRecvMsg(d);
    }

    idk_memcpy(dstBuf, decoder.getTail(), header.payloadSize);

    return goodRecvMsg(d);
}


bool idk::RemoteRxer::replyMsg(void *srcBuf, size_t srcSize)
{
    if (!mLastDatagram) { return false; }

    ByteEncoder encoder(mBuffer, sizeof(mBuffer));
    encoder.write(RemoteMessageHeader{mAuthToken, uint32_t(srcSize)});
    if (!encoder.write(srcBuf, srcSize))
    {
        VLOG_WARN("[RemoteRxer::replyMsg] Failed to send message: srcSize too large");
        return false;
    }
    if (!NET_SendDatagram(mSocket, mLastDatagram->addr, mLastDatagram->port, encoder.getBase(), encoder.getSize()))
    {
        VLOG_WARN("[RemoteRxer::replyMsg] Failed to send datagram: {}", SDL_GetError());
        return false;
    }
    return true;
}


NET_Datagram *idk::RemoteRxer::beginRecvMsg()
{
    NET_Datagram *d = nullptr;
    if (NET_ReceiveDatagram(mSocket, &d) && d)
    {
        if (d->buflen > 1500)
        {
            VLOG_WARN("[RemoteRxer::beginRecvMsg] datagram exceeds maximum allowed size");
            NET_DestroyDatagram(d);
            return nullptr;
        }
        return d;
    }
    return nullptr;
}

bool idk::RemoteRxer::goodRecvMsg(NET_Datagram *d)
{
    if (mLastDatagram)
    {
        NET_DestroyDatagram(mLastDatagram);
    }
    mLastDatagram = d;
    return true;
}

bool idk::RemoteRxer::badRecvMsg(NET_Datagram *d)
{
    NET_DestroyDatagram(d);
    return false;
}





idk::RemoteTxer::RemoteTxer(const char *hostname, uint16_t dstport)
:   mAuthToken(0xDEADBEBE),
    mDstPort(dstport)
{
    if (!NET_Init())
    {
        VLOG_FATAL("[RemoteTxer::RemoteTxer] Failure initializing SDL3_Net: {}", SDL_GetError());
    }

    mRemoteAddr = NET_ResolveHostname(hostname);
    if (!mRemoteAddr)
    {
        VLOG_FATAL("[RemoteTxer::RemoteTxer] Failure resolving host: {}", SDL_GetError());
    }
    NET_WaitUntilResolved(mRemoteAddr, 10000);

    if (!(mSocket = NET_CreateDatagramSocket(NULL, 0, 0)))
    {
        VLOG_FATAL("[RemoteTxer::RemoteTxer] Failure creating socket: {}", SDL_GetError());
    }
}


idk::RemoteTxer::~RemoteTxer()
{
    NET_UnrefAddress(mRemoteAddr);
    NET_DestroyDatagramSocket(mSocket);
}


bool idk::RemoteTxer::sendMsg(const void *srcBuf, size_t srcSize)
{
    ByteEncoder encoder(mBuffer, sizeof(mBuffer));
    encoder.write(RemoteMessageHeader{mAuthToken, uint32_t(srcSize)});
    if (!encoder.write(srcBuf, srcSize))
    {
        VLOG_WARN("[RemoteTxer::sendMsg] Failed to send message: srcSize too large");
        return false;
    }
    if (!NET_SendDatagram(mSocket, mRemoteAddr, mDstPort, encoder.getBase(), encoder.getSize()))
    {
        VLOG_WARN("[RemoteTxer::sendmsg] Failed to send datagram: {}", SDL_GetError());
        return false;
    }
    return true;
}
