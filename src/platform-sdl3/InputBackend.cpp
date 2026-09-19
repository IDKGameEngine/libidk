#include "libidk/platform-sdl3/InputBackend.hpp"
#include "libidk/platform-sdl3/PlatformContext.hpp"


idk::InputBackend::InputBackend()
{

}

void idk::InputBackend::update(idk::IPlatformContext *ctx)
{
    (void)ctx;
}

bool idk::InputBackend::isKeyDown(int keycode) const
{
    (void)keycode;
    return false;
}

bool idk::InputBackend::isMouseButtonDown(int button) const
{
    (void)button;
    return false;
}

void idk::InputBackend::getMousePosition(int& x, int& y) const
{
    x = 0;
    y = 0;
}
