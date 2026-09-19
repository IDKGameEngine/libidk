#pragma once

#include "libidk/platform/IInputBackend.hpp"


namespace idk
{
    class InputBackend: public idk::IInputBackend
    {
    private:

    public:
        InputBackend();
        virtual void update(idk::IPlatformContext*) final;
        virtual bool isKeyDown(int keycode) const final;
        virtual bool isMouseButtonDown(int button) const final;
        virtual void getMousePosition(int& x, int& y) const final;
    };

}

