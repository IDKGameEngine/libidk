#pragma once

#include "libidk/platform/IInputBackend.hpp"


namespace idk
{
    class InputBackend: public idk::IInputBackend
    {
    private:

    public:
        InputBackend();
        virtual void update(idk::BackendContext&) final;

    };

}

