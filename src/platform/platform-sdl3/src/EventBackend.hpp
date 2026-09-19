#pragma once

#include "libidk/platform/IEventBackend.hpp"


namespace idk
{
    class EventBackend: public idk::IEventBackend
    {
    private:

    public:
        EventBackend();
        virtual void update(idk::IPlatformContext&) final;

    };
}

