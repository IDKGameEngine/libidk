#include "libidk/message/RemoteRxTx.hpp"
#include "libidk/message/Wire.hpp"
#include <SDL3_net/SDL_net.h>
#include <cstdio>

void idk::RemoteEndpoint::assign(NET_Address *address, uint16_t port)
{
    if (address)
        NET_RefAddress(address);
    reset();
    mAddress = address;
    mPort = port;
}

idk::RemoteEndpoint::RemoteEndpoint(const RemoteEndpoint &other) { assign(other.mAddress, other.mPort); }
idk::RemoteEndpoint &idk::RemoteEndpoint::operator=(const RemoteEndpoint &other)
{
    assign(other.mAddress, other.mPort);
    return *this;
}
idk::RemoteEndpoint::~RemoteEndpoint() { reset(); }
void idk::RemoteEndpoint::reset()
{
    if (mAddress)
        NET_UnrefAddress(mAddress);
    mAddress = nullptr;
    mPort = 0;
}
bool idk::RemoteEndpoint::operator==(const RemoteEndpoint &other) const
{
    return mPort == other.mPort &&
           (mAddress == other.mAddress || (mAddress && other.mAddress && !NET_CompareAddresses(mAddress, other.mAddress)));
}

idk::RemoteRxTxer::RemoteRxTxer(const char *hostname, uint16_t port)
{
    open(hostname, hostname ? port : 0, hostname ? 0 : port);
}
idk::RemoteRxTxer::~RemoteRxTxer() { close(); }
void idk::RemoteRxTxer::recordError() { std::snprintf(mError, sizeof(mError), "%s", SDL_GetError()); }

bool idk::RemoteRxTxer::open(const char *hostname, uint16_t remotePort, uint16_t localPort, const char *bindHost)
{
    close();
    mError[0] = '\0';
    if (hostname && !remotePort)
    {
        std::snprintf(mError, sizeof(mError), "Remote port must be nonzero");
        return false;
    }
    if (!(mInitialized = NET_Init()))
    {
        recordError();
        return false;
    }
    NET_Address *destination = hostname ? NET_ResolveHostname(hostname) : nullptr;
    if (hostname && (!destination || NET_WaitUntilResolved(destination, 1000) != NET_SUCCESS))
    {
        recordError();
        if (destination)
            NET_UnrefAddress(destination);
        close();
        return false;
    }
    mRemote.assign(destination, remotePort);
    if (destination)
        NET_UnrefAddress(destination);
    NET_Address *binding = bindHost ? NET_ResolveHostname(bindHost) : nullptr;
    if (bindHost && (!binding || NET_WaitUntilResolved(binding, 1000) != NET_SUCCESS))
    {
        recordError();
        if (binding)
            NET_UnrefAddress(binding);
        close();
        return false;
    }
    SDL_PropertiesID properties = SDL_CreateProperties();
    if (properties)
    {
        SDL_SetBooleanProperty(properties, NET_PROP_DATAGRAM_SOCKET_REUSEADDR_BOOLEAN, false);
        mSocket = NET_CreateDatagramSocket(binding, localPort, properties);
        SDL_DestroyProperties(properties);
    }
    if (binding)
        NET_UnrefAddress(binding);
    if (!mSocket)
    {
        recordError();
        close();
        return false;
    }
    return true;
}

void idk::RemoteRxTxer::close()
{
    mRemote.reset();
    mSender.reset();
    mRecvInfo = {};
    if (mSocket)
        NET_DestroyDatagramSocket(mSocket);
    mSocket = nullptr;
    if (mInitialized)
        NET_Quit();
    mInitialized = false;
}

idk::RemoteReceive idk::RemoteRxTxer::receive(MessageRecvInfo *&message)
{
    message = nullptr;
    mSender.reset();
    if (!mSocket)
        return RemoteReceive::Error;
    NET_Datagram *packet = nullptr;
    if (!NET_ReceiveDatagram(mSocket, &packet))
    {
        recordError();
        close();
        return RemoteReceive::Error;
    }
    if (!packet)
        return RemoteReceive::Empty;
    bool valid = packet->buflen >= int(RemoteRxTx::HEADER_SIZE) &&
                 packet->buflen <= int(RemoteRxTx::HEADER_SIZE + RemoteRxTx::MAX_PAYLOAD_SIZE);
    MessageHeader header;
    if (valid)
    {
        WireReader reader(packet->buf, size_t(packet->buflen));
        header.authToken = reader.u64();
        header.messageSize = reader.u64();
        header.payloadSize = reader.u64();
        reader.bytes(header.payloadType.ascii, sizeof(header.payloadType.ascii));
        valid = reader.good() && header.authToken == RemoteRxTx::WIRE_VERSION &&
                header.messageSize == uint64_t(packet->buflen) &&
                header.payloadSize == uint64_t(packet->buflen) - RemoteRxTx::HEADER_SIZE;
        if (valid)
        {
            reader.bytes(mPayload, size_t(header.payloadSize));
            mSender.assign(packet->addr, packet->port);
            mRecvInfo.header = header;
            mRecvInfo.payload = mPayload;
            message = &mRecvInfo;
        }
    }
    NET_DestroyDatagram(packet);
    return valid ? RemoteReceive::Message : RemoteReceive::Invalid;
}

idk::MessageRecvInfo *idk::RemoteRxTxer::recvMsg()
{
    MessageRecvInfo *message = nullptr;
    receive(message);
    return message;
}

bool idk::RemoteRxTxer::sendMsg(const void *data, size_t size, MessagePayloadType type)
{
    return sendTo(mRemote.valid() ? mRemote : mSender, data, size, type);
}

bool idk::RemoteRxTxer::sendTo(const RemoteEndpoint &endpoint, const void *data, size_t size, MessagePayloadType type)
{
    if (!mSocket || !endpoint.valid() || size > RemoteRxTx::MAX_PAYLOAD_SIZE || (size && !data))
        return false;
    uint8_t buffer[RemoteRxTx::HEADER_SIZE + RemoteRxTx::MAX_PAYLOAD_SIZE];
    WireWriter writer(buffer, sizeof(buffer));
    writer.u64(RemoteRxTx::WIRE_VERSION);
    writer.u64(RemoteRxTx::HEADER_SIZE + size);
    writer.u64(size);
    writer.bytes(type.ascii, sizeof(type.ascii));
    writer.bytes(data, size);
    if (!NET_SendDatagram(mSocket, endpoint.mAddress, endpoint.mPort, buffer, int(writer.size())))
    {
        recordError();
        return false;
    }
    return true;
}

void idk::RemoteRxTxer::simulatePacketLoss(int percent)
{
    if (mSocket)
        NET_SimulateDatagramPacketLoss(mSocket, percent < 0 ? 0 : (percent > 100 ? 100 : percent));
}
