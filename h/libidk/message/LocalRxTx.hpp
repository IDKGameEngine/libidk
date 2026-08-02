#pragma once

#include "MessageRxTx.hpp"

namespace idk
{
    class LocalRxer: public idk::MessageRxer
    {
    public:
        LocalRxer(uint8_t port);
        virtual bool recvMsg(void *buf, size_t bufsz) override;

    private:
        uint8_t mPort;
        uint64_t mTick;
    };

    class LocalTxer: public idk::MessageTxer
    {
    public:
        LocalTxer(uint8_t port);
        virtual bool sendMsg(const void *buf, size_t bufsz) override;

    private:
        uint8_t mPort;
    };
}
