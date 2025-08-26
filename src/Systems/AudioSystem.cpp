#include "AudioSystem.h"
#include "../Core/Utility.h"
#include <SDL3/SDL_log.h>
#include <vector>

extern Coordinator gCoordinator;

void AudioSystem::Init()
{
    // Set audio spec
    SDL_AudioSpec audioSpec;
    SDL_zero( audioSpec );
    audioSpec.format = SDL_AUDIO_F32;
    audioSpec.channels = 2;
    audioSpec.freq = 44100;

    // Open Audio Device
    mAudioDeviceId = SDL_OpenAudioDevice( SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &audioSpec );
    if( mAudioDeviceId == 0 )
    {
        SDL_Log( "Unable to open audio! SDL error: %s\n", SDL_GetError() );
        // success = false;
    }
    else
    {
        //Initialize SDL_mixer
        if( Mix_OpenAudio( mAudioDeviceId, nullptr ) == false )
        {
            SDL_Log( "SDL_mixer could not initialize! SDL_mixer error: %s\n", SDL_GetError() );
            // success = false;
        }
    }
}

void AudioSystem::LoadAudio(Audio& audio)
{
    // If audio isn't loaded already
    if (mMusics.find(audio.filePath) == mMusics.end() && mSFXs.find(audio.filePath) == mSFXs.end())
    {
        // SDL_Log("TEST");
        bool success = true;
        std::string path = utility::getProjectPath(audio.filePath);

        if (audio.isMusic)
        {
            audio.music = Mix_LoadMUS(path.c_str());
            if (audio.music == nullptr)
            {
                SDL_Log( "Unable to load music! SDL_mixer error: %s\n", SDL_GetError() );
                success = false;
            }
        }
        else
        {
            audio.sfx = Mix_LoadWAV(path.c_str());
            if (audio.sfx == nullptr)
            {
                 SDL_Log( "Unable to load sfx! SDL_mixer error: %s\n", SDL_GetError() );
                success = false;
            }
        }

        if (success)
        {
            // Adding audio to map
            if (audio.isMusic)
            {
                mMusics.insert({audio.filePath, audio.music});
            }
            else
            {

                mSFXs.insert({audio.filePath, audio.sfx});
            }
            // mAudios.insert({audio.filePath, audio});

            // Allocate channels (not sure if music channel needs to be allocated aswell, maybe)
            mChannelCount = Mix_AllocateChannels(mSFXs.size() + mMusics.size());
            if (mChannelCount != mSFXs.size() + mMusics.size())
            {
                SDL_Log( "Unable to allocate channels! SDL_mixer error: %s\n", SDL_GetError() );
            }
        }
    }

    else
    {
        if (audio.isMusic)
        {
            audio.music = mMusics[audio.filePath];
        }
        else
        {
            audio.sfx = mSFXs[audio.filePath];
        }
    }
}


// TODO: Maybe add something to pause the music??
void AudioSystem::Update(float deltaTime)
{
    std::set<std::string> currentEntityAudios;
    int freeChannel = 0;

    // Loading audios and playing them
    for (const auto& entity : mEntities)
    {
        auto& audioComponent = gCoordinator.GetComponent<AudioComponent>(entity);

        for (auto it = audioComponent.audios.begin(); it != audioComponent.audios.end(); it++)
        {
            auto& audio = it->second;

            if (audio.isMusic)
            {
                // Loading Music if not loaded already
                if (audio.music == nullptr)
                {
                    LoadAudio(audio);
                }
            }
            else
            {
                // Load SFX if not loaded already
                if (audio.sfx == nullptr)
                {
                    LoadAudio(audio);
                }
            }

            // If it's already in playing it will overwrite it
            if (audio.isPlaying)
            {
                currentEntityAudios.insert(audio.filePath);
                mPlaying.insert(audio.filePath);
            }

            if (audio.isPlaying)
            {
                if (audio.isMusic)
                {
                    // Play music if not played
                    if (Mix_PlayingMusic() == 0)
                    {
                        // Music has its own channel, the int represents number of loops, -1 is infinite
                        Mix_PlayMusic(audio.music, -1);
                        // SDL_Log("PLAYING");
                    }
                }

                // For SFX
                else
                {
                    // Channel to play on, audio. int loops
                    Mix_PlayChannel(freeChannel, audio.sfx, 0);
                    freeChannel++;

                    // So they don't constantly play
                    audio.isPlaying = false;
                }
            }
            else
            {
                if (audio.isMusic)
                {
                    // Need to stop the music
                    Mix_HaltMusic();
                }
            }
        }
    }


    // If there is an audio playing thats not in the current entity's we need to stop it
    // For now I don't think sfx's matter so I will just do music
    std::vector<std::string> toBeErased;
    for (auto it = mPlaying.begin(); it != mPlaying.end(); it++)
    {
        std::string filePath = *it;
        if (currentEntityAudios.find(filePath) == currentEntityAudios.end())
        {
            if (Mix_PlayingMusic() != 0 && mMusics.find(filePath) != mMusics.end())
            {
                // SDL_Log("Stopping music");
                Mix_HaltMusic();
            }

            // Logic to erase sfx?
            else
            {}

            toBeErased.push_back(filePath);
        }
    }


    // Removing audios that are not playing from the playing set
    for (int i = 0; i < toBeErased.size(); i++)
    {
        mPlaying.erase(toBeErased[i]);
    }
}


void AudioSystem::Close()
{
    // Free music and SFX
    for (auto it : mMusics)
    {
        Mix_FreeMusic(it.second);
        // it.second = nullptr;
    }
    for (auto it : mSFXs)
    {
        Mix_FreeChunk(it.second);
        // it.second = nullptr;
    }
    // mAudios.clear();

    Mix_CloseAudio();
    SDL_CloseAudioDevice(mAudioDeviceId);
    mAudioDeviceId = 0;

    // Quit the SDL_Mixer subsystem
    Mix_Quit();
}
