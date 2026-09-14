#include "TimeBackend.hpp"
#include <SDL3/SDL_timer.h>

uint64_t idk::TimeBackend::getSysTimeMs()
{
    return SDL_GetTicks();
}

uint64_t idk::TimeBackend::getSysTimeNs()
{
    return SDL_GetTicksNS();
}

