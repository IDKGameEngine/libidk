#pragma once

#include "MessageRxTx.hpp"

struct NET_DatagramSocket;
struct NET_Address;
struct NET_Datagram;

namespace idk
{
    class RemoteRxer: public idk::MessageRxer
    {
    public:
        using MessageRxer::recvMsg;

        RemoteRxer(uint16_t port);
        virtual bool recvMsg(void *data, size_t size) override;

    private:
        const uint32_t mAuthToken;
        NET_DatagramSocket *mSocket;
        uint16_t mPort;
        // size_t mHeaderOffset;
        // size_t mPayloadOffset;
        uint8_t mBuffer[1500];

        NET_Datagram *beginRecvMsg();
        bool goodRecvMsg(NET_Datagram*);
        bool badRecvMsg(NET_Datagram*);
    };


    class RemoteTxer: public idk::MessageTxer
    {
    public:
        using MessageTxer::sendMsg;

        RemoteTxer(const char *hostname, uint16_t dstport);
        ~RemoteTxer();
        virtual bool sendMsg(const void *data, size_t size) override;

    private:
        const uint32_t mAuthToken;
        NET_DatagramSocket *mSocket;
        NET_Address *mRemoteAddr;
        uint16_t mDstPort;
        uint8_t mBuffer[1500];
    };
}

// #pragma once

// #include "MessageRxTx.hpp"

// struct NET_DatagramSocket;
// struct NET_Address;

// namespace idk
// {
//     class RemoteRxer: public idk::MessageRxer
//     {
//     public:
//         using MessageRxer::recvMsg;

//         RemoteRxer(uint16_t port);
//         virtual bool recvMsg(void *data, size_t size) override;

//     private:
//         const uint32_t mUdpMagic;
//         NET_DatagramSocket *mSocket;
//         uint16_t mPort;
//         uint8_t mBuf[1500];
//     };


//     class RemoteTxer: public idk::MessageTxer
//     {
//     public:
//         using MessageTxer::sendMsg;

//         RemoteTxer(const char *hostname, uint16_t dstport);
//         ~RemoteTxer();
//         virtual bool sendMsg(const void *data, size_t size) override;

//     private:
//         const uint32_t mUdpMagic;
//         NET_DatagramSocket *mSocket;
//         NET_Address *mRemoteAddr;
//         uint16_t mDstPort;
//         uint8_t mBuf[1500];
//     };
// }

