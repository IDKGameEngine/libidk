#include <SDL3_net/SDL_net.h>

#include "libidk/message/RemoteRxTx.hpp"

#include "libidk/stdmem.hpp"
#include "libidk/stdstr.hpp"
#include "libidk/math.hpp"
#include "libidk/log.hpp"



idk::RemoteRxer::RemoteRxer(uint16_t port)
:   mPort(port)
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
:   mDstPort(dstport)
{
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


bool idk::RemoteTxer::sendMsg(const void *data, size_t size)
{
    if (!mRemoteAddr)
    {
        VLOG_WARN("[RemoteTxer::sendmsg] Remote address is NULL");
        return false;
    }
    if (!NET_SendDatagram(mSocket, mRemoteAddr, mDstPort, data, size))
    {
        VLOG_WARN("[RemoteTxer::sendmsg] Failed to send datagram: {}", SDL_GetError());
        return false;
    }
    return true;
}

