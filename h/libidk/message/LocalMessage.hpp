#pragma once

#include "libidk/types.hpp"

namespace idk
{
    class LocalTxer: private idk::NonMobile
    {
    private:

    public:
        LocalTxer();
        ~LocalTxer();
        virtual bool sendMsg(const void *src, size_t size) = 0;
    };

    class LocalRxer: private idk::NonMobile
    {
    private:

    public:
        LocalRxer();
        ~LocalRxer();
        virtual bool recvMsg(void *dst, size_t size) = 0;
    };
}

