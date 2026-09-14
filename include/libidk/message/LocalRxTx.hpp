#pragma once

#include "MessageRxTx.hpp"

namespace idk
{
    namespace LocalRxTx
    {
        static constexpr size_t MAX_PAYLOAD_SIZE = 1024;

        struct PayloadBuffer
        {
            uint8_t data[LocalRxTx::MAX_PAYLOAD_SIZE];
        };

        struct MessageBuffer
        {
            MessageHeader header;
            PayloadBuffer payload;
        };
    }


    class LocalRxer: public idk::MessageRxer
    {
    public:
        LocalRxer(uint8_t port);
        virtual MessageRecvInfo *recvMsg() override;

    private:
        using PayloadBufType = LocalRxTx::PayloadBuffer;
        using MessageBufType = LocalRxTx::MessageBuffer;

        uint8_t         mPort;
        uint64_t        mTick;
        MessageHeader  &mHeader;
        PayloadBufType &mPayload;
        MessageBufType  mMsgBuf;
        MessageRecvInfo mRecvInfo;
    };


    class LocalTxer: public idk::MessageTxer
    {
    public:
        LocalTxer(uint8_t port);
        virtual bool sendMsg(const void *src, size_t size, MessagePayloadType type={0}) override;

    private:
        uint8_t mPort;
    };
}
