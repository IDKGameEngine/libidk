#pragma once

#include <cstddef>
#include <cstdint>

namespace idk
{
    class MessageRxer
    {
    public:
        MessageRxer() {}
        virtual ~MessageRxer() = default;
        virtual bool recvMsg(void *dst, size_t size) = 0;
    };

    class MessageTxer
    {
    public:
        MessageTxer() {}
        virtual ~MessageTxer() = default;
        virtual bool sendMsg(const void *src, size_t size) = 0;
    };
}

