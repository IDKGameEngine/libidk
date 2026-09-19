#pragma once

#include "libidk/platform/IAudioBackend.hpp"
#include "libidk/dsa/List.hpp"
#include "libidk/dsa/Stack.hpp"
#include <SDL3_mixer/SDL_mixer.h>


namespace idk
{
    class AudioBackend: public idk::IAudioBackend
    {
    public:
        static constexpr uint16_t MAX_SOUNDS = 64;

        class SoundType: public idk::IAudioBackend::ISoundType
        {
        private:
            friend class idk::AudioBackend;
            MIX_Audio *mAudio;
            MIX_Track *mTrack;
            uint16_t   mIdx;
        };

        AudioBackend();
        ~AudioBackend();

        virtual void update(idk::IPlatformContext*) final;
        virtual ISoundType *createSound(const char *filepath);
        virtual void destroySound(ISoundType*) final;
        virtual void startSound(ISoundType*) final;
        virtual void stopSound(ISoundType*) final;
        virtual void pauseSound(ISoundType*) final;
        virtual void resumeSound(ISoundType*) final;

    private:
        MIX_Mixer *mMixer;
    
        idk::ArrayType<SoundType, MAX_SOUNDS>   mSounds;
        idk::InplaceStack<uint16_t, MAX_SOUNDS> mFreelist;
        idk::InplaceStack<uint16_t, MAX_SOUNDS> mUsedlist;

    };

}
