#pragma once

#include "MessageRxTx.hpp"
#include "libidk/New.hpp"

namespace idk
{
    template <typename RxImpl, typename T>
    class PortRxer: public RxImpl
    {
    public:
        using RxImpl::RxImpl;
        bool readData(T &x) { return RxImpl::recvMsg((void*)&x, sizeof(T)); }
    };

    template <typename TxImpl, typename T>
    class PortTxer: public TxImpl
    {
    public:
        using TxImpl::TxImpl;
        bool sendData(const T &x) { return TxImpl::sendMsg((const void*)&x, sizeof(T)); }
    };

}
