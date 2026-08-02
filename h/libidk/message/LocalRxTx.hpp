#pragma once

#include "MessageRxTx.hpp"

namespace idk
{
    class LocalRxer: public idk::MessageRxer
    {
    public:
        LocalRxer(uint8_t port);
        virtual MessageRecvInfo *recvMsg() override;

    private:
        uint8_t mPort;
        uint64_t mTick;
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
