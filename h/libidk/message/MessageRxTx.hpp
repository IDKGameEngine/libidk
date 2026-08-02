#pragma once

#include "libidk/Types.hpp"

namespace idk
{
    class MessageRxer
    {
    public:
        virtual ~MessageRxer() = default;
        virtual bool recvMsg(void *dst, size_t size) = 0;
    
        template <typename T>
        bool recvMsg(T &x) { return recvMsg(&x, sizeof(T)); }
    };

    class MessageTxer
    {
    public:
        virtual ~MessageTxer() = default;
        virtual bool sendMsg(const void *src, size_t size) = 0;

        template <typename T>
        bool sendMsg(const T &x) { return sendMsg(&x, sizeof(T)); }
    };
}

