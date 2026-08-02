// #include <SDL3_net/SDL_net.h>

// #include "libidk/message/RemoteRxTx.hpp"

// #include "libidk/stdmem.hpp"
// #include "libidk/stdstr.hpp"
// #include "libidk/math.hpp"
// #include "libidk/log.hpp"


// struct Serializable
// {
// public:
//     virtual size_t serialize(uint8_t *dst) = 0;
//     virtual size_t deserialize(const uint8_t *src) = 0;

// protected:
//     template <typename T>
//     size_t write(uint8_t *dst, const T &x)
//     {
//         idk_memcpy(dst, &x, sizeof(T));
//         return sizeof(T);
//     }

//     template <typename T>
//     size_t read(const uint8_t *src, T &x)
//     {
//         idk_memcpy(&x, src, sizeof(T));
//         return sizeof(T);
//     }
// };


// struct RemoteRxTxHeader: public Serializable
// {
//     uint32_t authToken;
//     uint32_t payloadSize;
//     RemoteRxTxHeader(uint32_t auth=0, uint32_t size=0): authToken(auth), payloadSize(size) {  }

//     virtual size_t serialize(uint8_t *dst) final
//     {
//         size_t n = 0;
//         n += write(dst+n, authToken);
//         n += write(dst+n, payloadSize);
//         return n;
//     }

//     virtual size_t deserialize(const uint8_t *src) final
//     {
//         size_t n = 0;
//         n += read(src+n, authToken);
//         n += read(src+n, payloadSize);
//         return n;
//     }
// };



// idk::RemoteRxer::RemoteRxer(uint16_t port)
// :   mAuthToken(0xDEADBEBE),
//     mPort(port)
// {
//     if (!NET_Init())
//     {
//         VLOG_FATAL("[RemoteRxer::RemoteRxer] Failure initializing SDL3_Net: {}", SDL_GetError());
//     }

//     if (!(mSocket = NET_CreateDatagramSocket(NULL, mPort, 0)))
//     {
//         VLOG_FATAL("[RemoteRxer::RemoteRxer] Failure creating socket: {}", SDL_GetError());
//     }
// }


// bool idk::RemoteRxer::recvMsg(void *dstBuf, size_t dstSize)
// {
//     NET_Datagram *d = beginRecvMsg();
//     if (d == nullptr) { return false; }

//     RemoteRxTxHeader header;
//     size_t offset = header.deserialize(mBuffer);

//     if (header.authToken != mAuthToken)
//     {
//         return badRecvMsg(d);
//     }

//     if (header.payloadSize > dstSize)
//     {
//         VLOG_WARN("[RemoteRxer::recvMsg] payloadSize is larger than dstSize");
//         return badRecvMsg(d);
//     }

//     idk_memcpy(dstBuf, mBuffer+offset, header.payloadSize);
//     return goodRecvMsg(d);
// }


// NET_Datagram *idk::RemoteRxer::beginRecvMsg()
// {
//     NET_Datagram *d = nullptr;
//     if (NET_ReceiveDatagram(mSocket, &d) && d)
//     {
//         if (d->buflen > int(sizeof(mBuffer)))
//         {
//             VLOG_WARN("[RemoteRxer::beginRecvMsg] datagram size exceeds maximum allowed");
//             NET_DestroyDatagram(d);
//             return nullptr;
//         }
//         idk_memcpy(mBuffer, d->buf, d->buflen);
//         return d;
//     }
//     return nullptr;
// }

// bool idk::RemoteRxer::goodRecvMsg(NET_Datagram *d)
// {
//     NET_DestroyDatagram(d);
//     return true;
// }

// bool idk::RemoteRxer::badRecvMsg(NET_Datagram *d)
// {
//     NET_DestroyDatagram(d);
//     return false;
// }





// idk::RemoteTxer::RemoteTxer(const char *hostname, uint16_t dstport)
// :   mAuthToken(0xDEADBEBE),
//     mDstPort(dstport)
// {
//     if (!NET_Init())
//     {
//         VLOG_FATAL("[RemoteTxer::RemoteTxer] Failure initializing SDL3_Net: {}", SDL_GetError());
//     }

//     mRemoteAddr = NET_ResolveHostname(hostname);
//     if (!mRemoteAddr)
//     {
//         VLOG_FATAL("[RemoteTxer::RemoteTxer] Failure resolving host: {}", SDL_GetError());
//     }
//     NET_WaitUntilResolved(mRemoteAddr, 10000);

//     if (!(mSocket = NET_CreateDatagramSocket(NULL, 0, 0)))
//     {
//         VLOG_FATAL("[RemoteTxer::RemoteTxer] Failure creating socket: {}", SDL_GetError());
//     }
// }


// idk::RemoteTxer::~RemoteTxer()
// {
//     NET_DestroyDatagramSocket(mSocket);
// }


// bool idk::RemoteTxer::sendMsg(const void *srcBuf, size_t srcSize)
// {
//     size_t bufsz = 0;
//     bufsz += RemoteRxTxHeader(0xDEADBEBE).serialize(mBuffer+bufsz);
//     idk_memcpy(mBuffer+bufsz, srcBuf, srcSize);
//     bufsz += srcSize;

//     // if (!NET_SendDatagram(mSocket, mRemoteAddr, mDstPort, srcBuf, srcSize))
//     if (!NET_SendDatagram(mSocket, mRemoteAddr, mDstPort, mBuffer, bufsz))
//     {
//         VLOG_WARN("[RemoteTxer::sendmsg] Failed to send datagram: {}", SDL_GetError());
//         return false;
//     }
//     return true;
// }

#include <SDL3_net/SDL_net.h>

#include "libidk/message/RemoteRxTx.hpp"

#include "libidk/stdmem.hpp"
#include "libidk/stdstr.hpp"
#include "libidk/math.hpp"
#include "libidk/log.hpp"


struct Serializable
{
public:
    virtual size_t serialize(uint8_t *dst) = 0;
    virtual size_t deserialize(const uint8_t *src) = 0;

protected:
    template <typename T>
    size_t write(uint8_t *dst, const T &x)
    {
        idk_memcpy(dst, &x, sizeof(T));
        return sizeof(T);
    }

    template <typename T>
    size_t read(const uint8_t *src, T &x)
    {
        idk_memcpy(&x, src, sizeof(T));
        return sizeof(T);
    }
};


struct RemoteRxTxHeader: public Serializable
{
    uint32_t magic;
    uint32_t reserved;
    RemoteRxTxHeader(uint32_t m=0): magic(m), reserved(0) {  }

    virtual size_t serialize(uint8_t *dst) final
    {
        size_t n = 0;
        n += write(dst+n, magic);
        n += write(dst+n, reserved);
        return n;
    }

    virtual size_t deserialize(const uint8_t *src) final
    {
        size_t n = 0;
        n += read(src+n, magic);
        n += read(src+n, reserved);
        return n;
    }
};



idk::RemoteRxer::RemoteRxer(uint16_t port)
:   mUdpMagic(0xDEADBEBE),
    mPort(port)
{
    VLOG_INFO("[RemoteRxer] UDP_MAGIC={}", mUdpMagic);
    if (!NET_Init())
    {
        VLOG_FATAL("[RemoteRxer::RemoteRxer] Failure initializing SDL3_Net: {}", SDL_GetError());
    }

    if (!(mSocket = NET_CreateDatagramSocket(NULL, mPort, 0)))
    {
        VLOG_FATAL("[RemoteRxer::RemoteRxer] Failure creating socket: {}", SDL_GetError());
    }
}


bool idk::RemoteRxer::recvMsg(void *buf, size_t bufsz)
{
    NET_Datagram *dgram = NULL;
    while (NET_ReceiveDatagram(mSocket, &dgram) && dgram)
    {
        if (dgram->buflen > static_cast<int>(bufsz))
        {
            VLOG_WARN("[RemoteRxer::RemoteRxer] datagram->buflen > bufsz");
            NET_DestroyDatagram(dgram);
            continue;
        }
        int nbytes = dgram->buflen;
        idk_memcpy(buf, dgram->buf, nbytes);
        NET_DestroyDatagram(dgram);
        return true;
    }
    return false;
}





idk::RemoteTxer::RemoteTxer(const char *hostname, uint16_t dstport)
:   mUdpMagic(0xDEADBEBE),
    mDstPort(dstport)
{
    VLOG_INFO("[RemoteTxer] UDP_MAGIC={}", mUdpMagic);
    if (!NET_Init())
    {
        VLOG_FATAL("[RemoteTxer::RemoteTxer] Failure initializing SDL3_Net: {}", SDL_GetError());
    }

    mRemoteAddr = NET_ResolveHostname(hostname);
    NET_WaitUntilResolved(mRemoteAddr, 5000);
    if (!mRemoteAddr)
    {
        VLOG_FATAL("[RemoteTxer::RemoteTxer] Failure resolving host: {}", SDL_GetError());
    }

    if (!(mSocket = NET_CreateDatagramSocket(mRemoteAddr, 0, 0)))
    {
        VLOG_FATAL("[RemoteTxer::RemoteTxer] Failure creating socket: {}", SDL_GetError());
    }
}


idk::RemoteTxer::~RemoteTxer()
{
    NET_DestroyDatagramSocket(mSocket);
}


bool idk::RemoteTxer::sendMsg(const void *srcBuf, size_t srcSize)
{
    if (!mRemoteAddr)
    {
        VLOG_WARN("[RemoteTxer::sendmsg] Remote address is NULL");
        return false;
    }

    // size_t bufsz = 0;
    // bufsz += RemoteRxTxHeader(0xDEADBEBE).serialize(mBuf+bufsz);
    // idk_memcpy(mBuf+bufsz, srcBuf, srcSize);
    // bufsz += srcSize;

    // if (!NET_SendDatagram(mSocket, mRemoteAddr, mDstPort, mBuf, bufsz))
    if (!NET_SendDatagram(mSocket, mRemoteAddr, mDstPort, srcBuf, srcSize))
    {
        VLOG_WARN("[RemoteTxer::sendmsg] Failed to send datagram: {}", SDL_GetError());
        return false;
    }
    return true;
}

