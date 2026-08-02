#pragma once

#include "MessageRxTx.hpp"

struct NET_DatagramSocket;
struct NET_Address;
struct NET_Datagram;

namespace idk
{
    struct RemoteMessageHeader
    {
        uint32_t authToken;
        uint16_t payloadType;
        uint16_t payloadSize;
    };

    using RemoteMessagePayload = uint8_t[1024 - sizeof(RemoteMessageHeader)];

    struct RemoteMessageData
    {
        RemoteMessageHeader header;
        RemoteMessagePayload payload;
    };


    class RemoteRxer: public idk::MessageRxer
    {
    public:
        using MessageRxer::recvMsg;

        RemoteRxer(uint16_t port);
        virtual bool recvMsg(void *data, size_t size) override;
        bool replyMsg(uint16_t port, void *data, size_t size);

    private:
        const uint32_t        mAuthToken;
        NET_DatagramSocket   *mSocket;
        uint16_t              mPort;
        NET_Address          *mLastSender;
        RemoteMessageData     mMessageBuf;
        RemoteMessageHeader  &mHeader;
        RemoteMessagePayload &mPayload;

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
        const uint32_t        mAuthToken;
        NET_DatagramSocket   *mSocket;
        NET_Address          *mRemoteAddr;
        uint16_t              mDstPort;
        RemoteMessageData     mMessageBuf;
        RemoteMessageHeader  &mHeader;
        RemoteMessagePayload &mPayload;
    };



    class RemoteRxTxer: public idk::MessageRxer, public idk::MessageTxer
    {
    public:
        using MessageRxer::recvMsg;
        using MessageTxer::sendMsg;

        RemoteRxTxer(const char *hostname, uint16_t dstport);
        ~RemoteRxTxer();

        RemoteMessageData *recvMsg();

        virtual bool recvMsg(void *data, size_t size) override;
        virtual bool sendMsg(const void *data, size_t size) override;
        bool replyMsg(void *data, size_t size);

    private:
        const uint32_t        mAuthToken;
        NET_DatagramSocket   *mSocket;
        NET_Address          *mRemoteAddr;
        NET_Address          *mLastSender;
        uint16_t              mPort;
        RemoteMessageData     mMessageBuf;
        RemoteMessageHeader  &mHeader;
        RemoteMessagePayload &mPayload;

        NET_Datagram *beginRecvMsg();
        bool goodRecvMsg(NET_Datagram*);
        bool badRecvMsg(NET_Datagram*);
    };

}
