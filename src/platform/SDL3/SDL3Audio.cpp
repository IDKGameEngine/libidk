#include "libidk/platform/SDL3Audio.hpp"
#include "libidk/log.hpp"

#include <SDL3/SDL.h>


idk::SDL3Audio::SDL3Audio()
{
    if (false == MIX_Init())
    {
        VLOG_FATAL("{}", SDL_GetError());
    }

    mMixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
    IDK_ASSERT(mMixer != NULL, "{}", SDL_GetError());

    for (uint16_t i=0; i<MAX_SOUNDS; i++)
    {
        mFreelist.push(i);
    }
}

void idk::SDL3Audio::update(PlatformContext &ctx)
{
    (void)ctx;
}

idk::SDL3Audio::ISoundType *idk::SDL3Audio::createSound(const char *filepath)
{
    if (mFreelist.empty())
    {
        return nullptr;
    }

    uint16_t idx = mFreelist.top(); mFreelist.pop();
    mUsedlist.push(idx);

    SoundType *snd = &mSounds[idx];
    snd->mAudio = MIX_LoadAudio(mMixer, filepath, false);
    IDK_ASSERT(snd->mAudio != NULL, "[SDL3Audio::loadSound] {}", SDL_GetError());
    snd->mTrack = MIX_CreateTrack(mMixer);
    IDK_ASSERT(snd->mTrack != NULL, "[SDL3Audio::loadSound] {}", SDL_GetError());
    snd->mIdx = idx;
    MIX_SetTrackAudio(snd->mTrack, snd->mAudio);

    return snd;
}

void idk::SDL3Audio::destroySound(ISoundType *isnd)
{
    auto *snd = dynamic_cast<SoundType*>(isnd);
    MIX_DestroyTrack(snd->mTrack);
    MIX_DestroyAudio(snd->mAudio);
    mFreelist.push(snd->mIdx);
    mUsedlist.remove(snd->mIdx);
}

void idk::SDL3Audio::startSound(ISoundType *isnd)
{
    auto *snd = dynamic_cast<SoundType*>(isnd);
    MIX_PlayTrack(snd->mTrack, 0);
}

void idk::SDL3Audio::stopSound(ISoundType *isnd)
{
    auto *snd = dynamic_cast<SoundType*>(isnd);
    MIX_StopTrack(snd->mTrack, 0);
}

void idk::SDL3Audio::pauseSound(ISoundType *isnd)
{
    auto *snd = dynamic_cast<SoundType*>(isnd);
    MIX_PauseTrack(snd->mTrack);
}

void idk::SDL3Audio::resumeSound(ISoundType *isnd)
{
    auto *snd = dynamic_cast<SoundType*>(isnd);
    MIX_ResumeTrack(snd->mTrack);
}

