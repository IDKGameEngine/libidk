#pragma once

#include "MessageRxTx.hpp"

struct NET_DatagramSocket;
struct NET_Address;

namespace idk
{
    namespace RemoteRxTx
    {
        inline constexpr size_t MAX_PAYLOAD_SIZE = 1024;
        inline constexpr size_t HEADER_SIZE = 32;
        // Protocol marker and version, not an authentication credential.
        inline constexpr uint64_t WIRE_VERSION = 0x49444b4d00000001ULL;
    }

    // Owns a reference to an SDL_net address. Copies retain it.
    class RemoteEndpoint
    {
    public:
        RemoteEndpoint() = default;
        RemoteEndpoint(const RemoteEndpoint &);
        RemoteEndpoint &operator=(const RemoteEndpoint &);
        ~RemoteEndpoint();
        bool valid() const { return mAddress && mPort; }
        bool operator==(const RemoteEndpoint &) const;
        uint16_t port() const { return mPort; }
        void reset();

    private:
        friend class RemoteRxTxer;
        NET_Address *mAddress = nullptr;
        uint16_t mPort = 0;
        void assign(NET_Address *, uint16_t);
    };

    enum class RemoteReceive { Empty, Message, Invalid, Error };

    class RemoteRxTxer: public MessageRxer, public MessageTxer, public Immobile
    {
    public:
        RemoteRxTxer() = default;
        // Null hostname listens on port; a client binds an ephemeral local port.
        RemoteRxTxer(const char *hostname, uint16_t port);
        ~RemoteRxTxer() override;

        // Resolution may wait up to 1 second per address; polling never blocks.
        bool open(const char *hostname, uint16_t remotePort, uint16_t localPort = 0,
                  const char *bindHost = nullptr);
        void close();
        bool isOpen() const { return mSocket != nullptr; }
        const char *error() const { return mError; }
        const RemoteEndpoint &remote() const { return mRemote; }
        const RemoteEndpoint &sender() const { return mSender; }

        // Data and sender remain valid until the next receive or close. Sending
        // uses a separate buffer and never changes the configured destination.
        RemoteReceive receive(MessageRecvInfo *&);
        MessageRecvInfo *recvMsg() override;
        bool sendMsg(const void *, size_t, MessagePayloadType type = "") override;
        bool sendTo(const RemoteEndpoint &, const void *, size_t, MessagePayloadType type = "");
        void simulatePacketLoss(int percent);

    private:
        NET_DatagramSocket *mSocket = nullptr;
        bool mInitialized = false;
        RemoteEndpoint mRemote, mSender;
        uint8_t mPayload[RemoteRxTx::MAX_PAYLOAD_SIZE]{};
        MessageRecvInfo mRecvInfo{};
        char mError[256]{};
        void recordError();
    };
}
