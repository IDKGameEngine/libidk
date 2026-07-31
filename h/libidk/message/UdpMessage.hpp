#pragma once

#include "libidk/Types.hpp"

struct NET_DatagramSocket;
struct NET_Datagram;
struct NET_Address;

namespace idk
{
    class UdpRxTxer: private idk::Immobile
    {
    private:
        NET_DatagramSocket *mSocket;
        NET_Address *mRemoteAddr;
        uint16_t mRemotePort;

    public:
        UdpRxTxer(uint16_t localPort=0);
        UdpRxTxer(const char *hostname, uint16_t hostport);
        ~UdpRxTxer();
    
        NET_Datagram *beginRecvMsg(void *buf, size_t bufsz);
        void endRecvMsg(NET_Datagram*);
        void replyMsg(NET_Datagram*, const void *data, size_t size);
        virtual bool sendMsg(const void *data, size_t size) final;

    };
}
