#pragma once

#include <SDL3/SDL_audio.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <unordered_map>
#include "../Components/AudioComponent.h"
#include "../Core/ECS.h"
#include <set>
#include <string>

class AudioSystem : public System
{
public:
    void Init();
    void LoadAudio(Audio& audio);
    void Update(float deltaTime) override;
    void Close();

private:
    // Stores any audio we have loaded <path, audio>
    std::unordered_map<std::string, Mix_Music*> mMusics;
    std::unordered_map<std::string, Mix_Chunk*> mSFXs;

    std::set<std::string> mPlaying;

    // Playback audio device
    SDL_AudioDeviceID mAudioDeviceId = 0;
    int mChannelCount = 0;
};
