#pragma once

#include "MessageRxTx.hpp"

struct NET_DatagramSocket;
struct NET_Address;

namespace idk
{
    class RemoteRxer: public idk::MessageRxer
    {
    private:
        NET_DatagramSocket *mSocket;
        uint16_t mPort;
    public:
        RemoteRxer(uint16_t port);
        virtual bool recvMsg(void *data, size_t size) override;
    };

    class RemoteTxer: public idk::MessageTxer
    {
    private:
        NET_DatagramSocket *mSocket;
        NET_Address *mRemoteAddr;
        uint16_t mDstPort;
    public:
        RemoteTxer(const char *hostname, uint16_t dstport);
        ~RemoteTxer();
        virtual bool sendMsg(const void *data, size_t size) override;
    };
}

