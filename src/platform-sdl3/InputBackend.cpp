#include "libidk/platform-sdl3/InputBackend.hpp"
#include "libidk/platform-sdl3/PlatformContext.hpp"

#include <SDL3/SDL.h>


idk::InputBackend::InputBackend()
{

}

void idk::InputBackend::update(idk::IPlatformContext *ctx)
{
    (void)ctx;

    // SDL_GetKeyboardState();
    // SDL_GetGamepadAxis()
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
