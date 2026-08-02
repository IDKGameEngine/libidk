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
    mPort(port),
    mLastSender(nullptr),
    mHeader(mMessageBuf.header),
    mPayload(mMessageBuf.payload)
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
    decoder.read(&mHeader, sizeof(mHeader));
    if (mHeader.authToken != mAuthToken)
    {
        VLOG_WARN("[RemoteRxer::recvMsg] header.authToken != mAuthToken");
        return badRecvMsg(d);
    }
    if (mHeader.payloadSize > dstSize)
    {
        VLOG_WARN("[RemoteRxer::recvMsg] payloadSize is larger than dstSize");
        return badRecvMsg(d);
    }
    decoder.read(dstBuf, dstSize);

    if (mLastSender)
    {
        NET_UnrefAddress(mLastSender);
    }
    mLastSender = NET_RefAddress(d->addr);

    return goodRecvMsg(d);
}

bool idk::RemoteRxer::replyMsg(uint16_t port, void *srcBuf, size_t srcSize)
{
    if (!mLastSender)
    {
        VLOG_WARN("[RemoteRxer::replyMsg] Failed to reply message: no previous sender");
        return false;
    }

    ByteEncoder encoder(&mMessageBuf, sizeof(mMessageBuf));
    encoder.write(RemoteMessageHeader{mAuthToken, 0, uint16_t(srcSize)});
    if (!encoder.write(srcBuf, srcSize))
    {
        VLOG_WARN("[RemoteRxer::replyMsg] Failed to send message: srcSize too large");
        return false;
    }
    if (!NET_SendDatagram(mSocket, mLastSender, port, encoder.getBase(), encoder.getSize()))
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
    NET_DestroyDatagram(d);
    return true;
}

bool idk::RemoteRxer::badRecvMsg(NET_Datagram *d)
{
    NET_DestroyDatagram(d);
    return false;
}





idk::RemoteTxer::RemoteTxer(const char *hostname, uint16_t dstport)
:   mAuthToken(0xDEADBEBE),
    mDstPort(dstport),
    mHeader(mMessageBuf.header),
    mPayload(mMessageBuf.payload)
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
    ByteEncoder encoder(&mMessageBuf, sizeof(mMessageBuf));
    encoder.write(RemoteMessageHeader{mAuthToken, 0, uint16_t(srcSize)});
    if (!encoder.write(&srcBuf, srcSize))
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







idk::RemoteRxTxer::RemoteRxTxer(const char *hostname, uint16_t port)
:   mAuthToken(0xDEADBEBE),
    mSocket(nullptr),
    mRemoteAddr(nullptr),
    mLastSender(nullptr),
    mPort(port),
    mHeader(mMessageBuf.header),
    mPayload(mMessageBuf.payload)
{
    if (!NET_Init())
    {
        VLOG_FATAL("[RemoteRxTxer::RemoteRxTxer] Failure initializing SDL3_Net: {}", SDL_GetError());
    }

    if (hostname)
    {
        mRemoteAddr = NET_ResolveHostname(hostname);
        if (!mRemoteAddr)
        {
            VLOG_FATAL("[RemoteRxTxer::RemoteRxTxer] Failure resolving host: {}", SDL_GetError());
        }
        NET_WaitUntilResolved(mRemoteAddr, 10000);
    }

    if (!(mSocket = NET_CreateDatagramSocket(NULL, port, 0)))
    {
        VLOG_FATAL("[RemoteTxer::RemoteRxTxer] Failure creating socket: {}", SDL_GetError());
    }
}


idk::RemoteRxTxer::~RemoteRxTxer()
{
    if (mRemoteAddr) { NET_UnrefAddress(mRemoteAddr); }
    if (mSocket)     { NET_DestroyDatagramSocket(mSocket); }
}


bool idk::RemoteRxTxer::recvMsg(void *dstBuf, size_t dstSize)
{
    NET_Datagram *d = beginRecvMsg();
    if (d == nullptr) { return false; }

    idk_memcpy(&mHeader, d->buf, sizeof(mHeader));
    if (mHeader.authToken != mAuthToken)
    {
        VLOG_WARN("[RemoteRxTxer::recvMsg] header.authToken != mAuthToken");
        return badRecvMsg(d);
    }
    if (mHeader.payloadSize > dstSize)
    {
        VLOG_WARN("[RemoteRxTxer::recvMsg] payloadSize is larger than dstSize");
        return badRecvMsg(d);
    }
    uint8_t *payload = (uint8_t*)(d->buf) + sizeof(mHeader);
    idk_memcpy(dstBuf, payload, mHeader.payloadSize);

    if (mLastSender)
    {
        NET_UnrefAddress(mLastSender);
    }
    mLastSender = NET_RefAddress(d->addr);

    return goodRecvMsg(d);
}


bool idk::RemoteRxTxer::sendMsg(const void *srcBuf, size_t srcSize)
{
    mHeader = {mAuthToken, 0, uint16_t(srcSize)};
    if (srcSize > sizeof(mPayload))
    {
        VLOG_WARN("[RemoteRxTxer::sendMsg] Failed to send message: srcSize too large");
        return false;
    }
    idk_memcpy(&mPayload, srcBuf, srcSize);
    if (!NET_SendDatagram(mSocket, mRemoteAddr, mPort, &mMessageBuf, sizeof(mHeader) + srcSize))
    {
        VLOG_WARN("[RemoteRxTxer::sendmsg] Failed to send datagram: {}", SDL_GetError());
        return false;
    }
    return true;
}


bool idk::RemoteRxTxer::replyMsg(void *srcBuf, size_t srcSize)
{
    if (!mLastSender)
    {
        VLOG_WARN("[RemoteRxTxer::replyMsg] Failed to reply message: no previous sender");
        return false;
    }

    mHeader = {mAuthToken, 0, uint16_t(srcSize)};
    if (srcSize > sizeof(mPayload))
    {
        VLOG_WARN("[RemoteRxTxer::sendMsg] Failed to send message: srcSize too large");
        return false;
    }
    idk_memcpy(&mPayload, srcBuf, srcSize);

    if (!NET_SendDatagram(mSocket, mLastSender, mPort, &mMessageBuf, sizeof(mHeader) + srcSize))
    {
        VLOG_WARN("[RemoteRxTxer::replyMsg] Failed to send datagram: {}", SDL_GetError());
        return false;
    }
    return true;
}


NET_Datagram *idk::RemoteRxTxer::beginRecvMsg()
{
    NET_Datagram *d = nullptr;
    if (NET_ReceiveDatagram(mSocket, &d) && d)
    {
        if (d->buflen > 1500)
        {
            VLOG_WARN("[RemoteRxTxer::beginRecvMsg] datagram exceeds maximum allowed size");
            NET_DestroyDatagram(d);
            return nullptr;
        }
        return d;
    }
    return nullptr;
}

bool idk::RemoteRxTxer::goodRecvMsg(NET_Datagram *d)
{
    NET_DestroyDatagram(d);
    return true;
}

bool idk::RemoteRxTxer::badRecvMsg(NET_Datagram *d)
{
    NET_DestroyDatagram(d);
    return false;
}


