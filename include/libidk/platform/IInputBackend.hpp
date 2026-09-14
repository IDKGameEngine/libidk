#pragma once

#include "IPlatformFeature.hpp"


namespace idk
{
    class IInputBackend: public idk::IPlatformFeature
    {
    private:

    public:
        virtual bool isKeyDown(int keycode) const = 0;
        virtual bool isMouseButtonDown(int button) const = 0;
        virtual void getMousePosition(int& x, int& y) const = 0;
    };

}

