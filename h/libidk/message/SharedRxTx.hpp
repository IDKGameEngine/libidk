#pragma once

#include "MessageRxTx.hpp"

namespace idk
{
    class SharedRxer: public idk::MessageRxer
    {
    public:
        SharedRxer(const char *label);
        virtual bool recvMsg(void *buf, size_t bufsz) override;

    private:
        void *mRegion;
        int64_t mTick;
    };

    class SharedTxer: public idk::MessageTxer
    {
    public:
        SharedTxer(const char *label);
        virtual bool sendMsg(const void *buf, size_t bufsz) override;

    private:
        void *mRegion;
    };
}

