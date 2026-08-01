#pragma once

#include "libidk/Types.hpp"

namespace idk
{
    class MessageRxer
    {
    public:
        virtual ~MessageRxer() = default;
        virtual bool recvMsg(void *dst, size_t size) = 0;
    };

    class MessageTxer
    {
    public:
        virtual ~MessageTxer() = default;
        virtual bool sendMsg(const void *src, size_t size) = 0;
    };
}

