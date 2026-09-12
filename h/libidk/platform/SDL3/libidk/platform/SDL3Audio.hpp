#pragma once

#include "libidk/platform/IPlatformAudio.hpp"
#include "libidk/dsa/List.hpp"
#include "libidk/dsa/Stack.hpp"
#include <SDL3_mixer/SDL_mixer.h>


namespace idk
{
    class SDL3Audio: public IPlatformAudio
    {
    public:
        static constexpr uint16_t MAX_SOUNDS = 64;

        class SoundType: public IPlatformAudio::ISoundType
        {
        private:
            friend class idk::SDL3Audio;
            MIX_Audio *mAudio;
            MIX_Track *mTrack;
            uint16_t   mIdx;
        };

        SDL3Audio();
        virtual void update(PlatformContext&) final;
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
