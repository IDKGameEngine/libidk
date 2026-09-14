#include "libidk/platform-sdl3/EventBackend.hpp"
#include <SDL3/SDL.h>


idk::EventBackend::EventBackend()
{
    if (false == SDL_Init(SDL_INIT_EVENTS))
    {
        VLOG_FATAL("{}", SDL_GetError());
    }
}


void idk::EventBackend::update(idk::BackendContext &ctx)
{
    (void)ctx;

    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        for (uintptr_t addr: mEventFuncs)
        {
            reinterpret_cast<EventCallback>(addr)(this, &e);
        }

        if (e.type == SDL_EVENT_QUIT)
        {
            ctx.shutdown();
        }

        if (e.type == SDL_EVENT_KEY_UP)
        {
            if (e.key.scancode == SDL_SCANCODE_ESCAPE)
            {
                ctx.shutdown();
            }
        }
    }
}

