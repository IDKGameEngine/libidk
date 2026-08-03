#pragma once

#include "libidk/Types.hpp"

namespace idk
{
    class PlatformAudio: public idk::Immobile
    {
    private:

    public:
        PlatformAudio(const char *title, int w, int h);
        ~PlatformAudio();

    };
}
