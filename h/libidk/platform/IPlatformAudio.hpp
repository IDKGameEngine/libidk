#pragma once

#include "PlatformContext.hpp"


namespace idk
{
    class IPlatformAudio: public IPlatformFeature
    {
    private:
        
    public:
        class ISoundType
        {
        public:
            ISoundType() {  };
            virtual ~ISoundType() = default;
        };

        IPlatformAudio() {  };
        virtual ISoundType *createSound(const char *filepath) = 0;
        virtual void destroySound(ISoundType*) = 0;
        virtual void startSound(ISoundType*) = 0;
        virtual void stopSound(ISoundType*) = 0;
        virtual void pauseSound(ISoundType*) = 0;
        virtual void resumeSound(ISoundType*) = 0;

    };
}
