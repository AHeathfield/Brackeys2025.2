#pragma once

#include <SDL3/SDL_log.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <string>
#include <unordered_map>

struct Audio
{
    Mix_Chunk* sfx = nullptr;
    Mix_Music* music = nullptr;
    std::string filePath;
    bool isMusic = false;
    bool isPlaying = false;
};

struct AudioComponent
{
    std::unordered_map<std::string, Audio> audios;

    void Play(std::string audioName)
    {
        if (audios.find(audioName) != audios.end())
        {
            audios[audioName].isPlaying = true;
        }
        else
        {
            SDL_Log("Audio name does not exist");
        }
    }

    void Stop(std::string audioName)
    {
        if (audios.find(audioName) != audios.end())
        {
            audios[audioName].isPlaying = false;
        }
        else
        {
            SDL_Log("Audio name does not exist");
        }
    }
};
