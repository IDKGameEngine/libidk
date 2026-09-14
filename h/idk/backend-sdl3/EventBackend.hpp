#pragma once

#include "idk/backend/IEventBackend.hpp"


namespace idk
{
    class EventBackend: public idk::IEventBackend
    {
    private:

    public:
        EventBackend();
        virtual void update(idk::BackendContext&) final;

    };
}

