#pragma once

#include "MessageRxTx.hpp"

namespace idk
{
    class SharedRxer: public idk::MessageRxer
    {
    public:
        using MessageRxer::recvMsg;

        SharedRxer(const char *name, size_t size);
        virtual bool recvMsg(void *buf, size_t bufsz) override;

    private:
        void *mRegion;
        uint64_t mTick;
    };

    class SharedTxer: public idk::MessageTxer
    {
    public:
        using MessageTxer::sendMsg;

        SharedTxer(const char *name, size_t size);
        virtual bool sendMsg(const void *buf, size_t bufsz) override;

    private:
        void *mRegion;
    };
}

