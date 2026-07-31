#include <SDL3_net/SDL_net.h>

#include "libidk/message/UdpMessage.hpp"

#include "libidk/stdmem.hpp"
#include "libidk/stdstr.hpp"
#include "libidk/math.hpp"
#include "libidk/log.hpp"


idk::UdpRxTxer::UdpRxTxer(uint16_t localPort)
:   mRemoteAddr(NULL), mRemotePort(0)
{
    if (!(mSocket = NET_CreateDatagramSocket(NULL, localPort, 0)))
    {
        VLOG_FATAL("[UdpRxTxer::UdpRxTxer] Failure creating socket: {}", SDL_GetError());
    }
}


idk::UdpRxTxer::UdpRxTxer(const char *hostname, uint16_t hostport)
:   UdpRxTxer(0)
{
    if (!(mRemoteAddr = NET_ResolveHostname(hostname)))
    {
        VLOG_FATAL("[UdpRxTxer::UdpRxTxer] Failure resolving host: {}", SDL_GetError());
    }
    mRemotePort = hostport;
}


idk::UdpRxTxer::~UdpRxTxer()
{
    VLOG_INFO("[UdpRxTxer::~UdpRxTxer]");
    NET_DestroyDatagramSocket(mSocket);
}


NET_Datagram *idk::UdpRxTxer::beginRecvMsg(void *buf, size_t bufsz)
{
    NET_Datagram *dgram = NULL;
    if (NET_ReceiveDatagram(mSocket, &dgram) && dgram)
    {
        if (dgram->buflen > static_cast<int>(bufsz))
        {
            VLOG_WARN("[UdpRxTxer::beginRecvMsg] datagram->buflen > bufsz");
            return nullptr;
        }
        int nbytes = dgram->buflen;
        idk_memcpy(buf, dgram->buf, nbytes);
        return dgram;
    }
    return nullptr;
}


void idk::UdpRxTxer::endRecvMsg(NET_Datagram *dgram)
{
    NET_DestroyDatagram(dgram);
}


void idk::UdpRxTxer::replyMsg(NET_Datagram *dgram, const void *data, size_t size)
{
    if (!NET_SendDatagram(mSocket, dgram->addr, dgram->port, data, size))
    {
        VLOG_WARN("[UdpRxTxer::sendmsg] Failed to send datagram: {}", SDL_GetError());
    }
}


bool idk::UdpRxTxer::sendMsg(const void *data, size_t size)
{
    if (!mRemoteAddr)
    {
        VLOG_WARN("[UdpRxTxer::sendmsg] Remote address is NULL");
        return false;
    }
    if (!NET_SendDatagram(mSocket, mRemoteAddr, mRemotePort, data, size))
    {
        VLOG_WARN("[UdpRxTxer::sendmsg] Failed to send datagram: {}", SDL_GetError());
        return false;
    }
    return true;
}

