#include "libidk/platform/SDL3Audio.hpp"
#include "libidk/log.hpp"

#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>


idk::SDL3Audio::SDL3Audio()
{
    if (false == SDL_Init(SDL_INIT_AUDIO))
    {
        VLOG_FATAL("{}", SDL_GetError());
    }
}


void idk::SDL3Audio::update(PlatformContext &ctx)
{

}

