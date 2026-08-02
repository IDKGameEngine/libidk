#pragma once

#include "MessageRxTx.hpp"

struct NET_DatagramSocket;
struct NET_Address;
struct NET_Datagram;

namespace idk
{
    namespace RemoteRxTx
    {
        static constexpr size_t MAX_PAYLOAD_SIZE = 1024;

        struct PayloadBuffer
        {
            uint8_t data[RemoteRxTx::MAX_PAYLOAD_SIZE];
        };

        struct MessageBuffer
        {
            MessageHeader header;
            PayloadBuffer payload;
        };
    }


    // class RemoteRxer: public idk::MessageRxer
    // {
    // public:
    //     using MessageRxer::recvMsg;

    //     RemoteRxer(uint16_t port);
    //     virtual bool recvMsg(void *data, size_t size) override;
    //     bool replyMsg(uint16_t port, void *data, size_t size);

    // private:
    //     const uint32_t        mAuthToken;
    //     NET_DatagramSocket   *mSocket;
    //     uint16_t              mPort;
    //     NET_Address          *mLastSender;
    //     RemoteMessageData     mMessageBuf;
    //     RemoteMessageHeader  &mHeader;
    //     RemoteMessagePayload &mPayload;

    //     NET_Datagram *beginRecvMsg();
    //     bool goodRecvMsg(NET_Datagram*);
    //     bool badRecvMsg(NET_Datagram*);
    // };


    // class RemoteTxer: public idk::MessageTxer
    // {
    // public:
    //     using MessageTxer::sendMsg;

    //     RemoteTxer(const char *hostname, uint16_t dstport);
    //     ~RemoteTxer();
    //     virtual bool sendMsg(const void *data, size_t size) override;

    // private:
    //     const uint32_t        mAuthToken;
    //     NET_DatagramSocket   *mSocket;
    //     NET_Address          *mRemoteAddr;
    //     uint16_t              mDstPort;
    //     RemoteMessageData     mMessageBuf;
    //     RemoteMessageHeader  &mHeader;
    //     RemoteMessagePayload &mPayload;
    // };



    class RemoteRxTxer: public idk::MessageRxer, public idk::MessageTxer
    {
    public:
        using MessageRxer::recvMsg;
        using MessageTxer::sendMsg;

        RemoteRxTxer(const char *hostname, uint16_t dstport);
        ~RemoteRxTxer();
    
        virtual MessageRecvInfo *recvMsg() override;
        virtual bool sendMsg(const void *src, size_t size, MessagePayloadType type={0}) override;


    private:
        using PayloadBufType = RemoteRxTx::PayloadBuffer;
        using MessageBufType = RemoteRxTx::MessageBuffer;

        const uint32_t        mAuthToken;
        NET_DatagramSocket   *mSocket;
        NET_Address          *mRemoteAddr;
        NET_Address          *mLastSender;
        uint16_t              mPort;
        MessageHeader        &mHeader;
        PayloadBufType       &mPayload;
        MessageBufType        mMsgBuf;
        MessageRecvInfo       mRecvInfo;

        NET_Datagram *beginRecvMsg();
        MessageRecvInfo *goodRecvMsg(NET_Datagram*);
        MessageRecvInfo *badRecvMsg(NET_Datagram*);
    };

}
