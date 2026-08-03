#include "libidk/platform/SDL3Time.hpp"
#include <SDL3/SDL_timer.h>

uint64_t idk::SDL3Time::getSysTimeMs()
{
    return SDL_GetTicks();
}

uint64_t idk::SDL3Time::getSysTimeNs()
{
    return SDL_GetTicksNS();
}

