#pragma once

#include "libidk/Types.hpp"
#include "libidk/assert.hpp"
#include "libidk/stdmem.hpp"
#include "libidk/stdstr.hpp"

namespace idk
{
    union MessagePayloadType
    {
        uint64_t value;
        char ascii[sizeof(value)];
    
        MessagePayloadType(const char *str)
        {
            static constexpr size_t ASCII_STRLEN = sizeof(ascii) - 1;
            IDK_ASSERT(idk_strlen(str) <= ASCII_STRLEN, "Ascii ID exceeds maximum allowed length");
            idk_strncpy(ascii, str, ASCII_STRLEN);
            ascii[ASCII_STRLEN] = '\0';
        }
    };

    struct MessageHeader
    {
        uint64_t           authToken;
        uint64_t           messageSize; // Size of entire message: sizeof(MessageHeader) + payloadSize
        uint64_t           payloadSize; // Size of payload data
        MessagePayloadType payloadType;
    
        MessageHeader(uint64_t auth=0, uint64_t size=0, MessagePayloadType type="\0")
        :   authToken(auth), messageSize(0), payloadSize(size), payloadType(type)
        {
            messageSize = sizeof(MessageHeader) + payloadSize;
        }
    };

    struct MessageRecvInfo
    {
        MessageHeader header;
        void         *payload;

        bool isType(const char *str)
        {
            auto &ascii = header.payloadType.ascii;
            return 0 == idk_strncmp(ascii, str, sizeof(ascii));
        }
    };

    class MessageRxer
    {
    public:
        virtual ~MessageRxer() = default;
        virtual MessageRecvInfo *recvMsg() = 0;
    
        // template <typename T>
        // bool recvMsg(T &x) { return recvMsg(&x, sizeof(T)); }
    };

    class MessageTxer
    {
    public:
        virtual ~MessageTxer() = default;
        virtual bool sendMsg(const void *src, size_t size, MessagePayloadType type={0}) = 0;

        // template <typename T>
        // bool sendMsg(const T &x) { return sendMsg(&x, sizeof(T)); }
    };
}

