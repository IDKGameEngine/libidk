#include "libidk/platform-sdl3/AudioBackend.hpp"
#include "libidk/platform-sdl3/PlatformContext.hpp"
#include "libidk/log.hpp"
#include <SDL3/SDL.h>


idk::AudioBackend::AudioBackend()
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

void idk::AudioBackend::update(idk::IPlatformContext *ctx)
{
    (void)ctx;
}

idk::AudioBackend::ISoundType *idk::AudioBackend::createSound(const char *filepath)
{
    if (mFreelist.empty())
    {
        VLOG_WARN("[AudioBackend::createSound] Cannot create any more sounds!");
        return nullptr;
    }

    uint16_t idx = mFreelist.top(); mFreelist.pop();
    mUsedlist.push(idx);

    SoundType *snd = &mSounds[idx];
    snd->mAudio = MIX_LoadAudio(mMixer, filepath, false);
    IDK_ASSERT(snd->mAudio != NULL, "[AudioBackend::loadSound] {}", SDL_GetError());
    snd->mTrack = MIX_CreateTrack(mMixer);
    IDK_ASSERT(snd->mTrack != NULL, "[AudioBackend::loadSound] {}", SDL_GetError());
    snd->mIdx = idx;
    MIX_SetTrackAudio(snd->mTrack, snd->mAudio);

    return snd;
}

void idk::AudioBackend::destroySound(ISoundType *isnd)
{
    auto *snd = dynamic_cast<SoundType*>(isnd);
    MIX_DestroyTrack(snd->mTrack);
    MIX_DestroyAudio(snd->mAudio);
    mFreelist.push(snd->mIdx);
    mUsedlist.remove(snd->mIdx);
}

void idk::AudioBackend::startSound(ISoundType *isnd)
{
    auto *snd = dynamic_cast<SoundType*>(isnd);
    MIX_PlayTrack(snd->mTrack, 0);
}

void idk::AudioBackend::stopSound(ISoundType *isnd)
{
    auto *snd = dynamic_cast<SoundType*>(isnd);
    MIX_StopTrack(snd->mTrack, 0);
}

void idk::AudioBackend::pauseSound(ISoundType *isnd)
{
    auto *snd = dynamic_cast<SoundType*>(isnd);
    MIX_PauseTrack(snd->mTrack);
}

void idk::AudioBackend::resumeSound(ISoundType *isnd)
{
    auto *snd = dynamic_cast<SoundType*>(isnd);
    MIX_ResumeTrack(snd->mTrack);
}

