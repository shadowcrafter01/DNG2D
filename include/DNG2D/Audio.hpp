#ifndef DNG_AUDIO_HPP
#define DNG_AUDIO_HPP

#include <SDL3_mixer/SDL_mixer.h>

// #include "Main.hpp"
#include "DNG2D/Console.hpp"

#include <vector>
#include <array>
#include <algorithm>

#define AUDIO_TRACK_POOL_COUNT 64

namespace DNG
{

    class Audio
    {
    private:
        static inline MIX_Mixer *_mixer = nullptr;
        static inline bool _flag_mixerInit = false;

        MIX_Audio *_audio;
        MIX_Track *_track;
        static inline std::array<MIX_Track *, AUDIO_TRACK_POOL_COUNT> _allTracks = {};
        bool _state = false;

        float _volume = 1.0f;
        float _pitch = 1.0f;

    public:
        Audio(const char *path)
        {
            if (!_flag_mixerInit)
            {
                Console::LogLoadStart("Initializing mixer");
                if (!MIX_Init())
                {
                    Console::LogLoadEnd(false);
                    return;
                }
                _mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
                if (_mixer == NULL)
                {
                    Console::LogLoadEnd(false);
                    return;
                }
                for (auto &t : _allTracks)
                {
                    t = MIX_CreateTrack(_mixer);
                    if (t == NULL)
                    {
                        Console::LogLoadEnd(false);
                        return;
                    }
                }
                Console::LogLoadEnd(true);
                _flag_mixerInit = true;
            }
            Console::LogLoadStart((std::string) "Loading audio file [" + path + "]");
            _audio = MIX_LoadAudio(_mixer, path, false);
            if (_audio == NULL)
            {
                Console::LogLoadEnd(false);
                return;
            }

            _track = MIX_CreateTrack(_mixer);
            if (_track == NULL)
            {
                Console::LogLoadEnd(false);
                return;
            }

            MIX_SetTrackAudio(_track, _audio);

            Console::LogLoadEnd(true);
            _state = true;
        }
        ~Audio()
        {
        }

        /// @brief starts the sound clip, if it was already playing then restarts, call the static play if you want to overlap multiple of the same audio clip
        /// @param volume volume duh
        /// @param pitch pitch/playback speed
        /// @return
        Audio *Play(float volume = 1.0f, float pitch = 1.0f)
        {
            if (volume != _volume)
            {
                SetVolume(volume);
            }
            if (pitch != _pitch)
            {
                SetPitch(pitch);
            }
            MIX_PlayTrack(_track, 0);
            return this;
        }
        /// @brief starts a new audio clip on its own channel, subsequent calls start more new sounds that overlap unlike calling Play() on the object directly
        /// @param audio audio to play
        /// @param volume volume duh
        /// @param pitch pitch/playback speed
        static void Play(Audio *audio, float volume = 1.0f, float pitch = 1.0f)
        {
            if (_mixer == NULL || audio->_audio == NULL)
            {
                return;
            }
            int i = 0;
            for (auto *t : _allTracks)
            {
                i++;
                if (i != AUDIO_TRACK_POOL_COUNT)
                {
                    if (t == NULL || MIX_TrackPlaying(t))
                    {
                        continue;
                    }
                }
                if (!MIX_SetTrackAudio(t, audio->_audio))
                {
                    return;
                }
                MIX_SetTrackGain(t, volume);
                MIX_SetTrackFrequencyRatio(t, pitch);
                MIX_PlayTrack(t, 0);
                return;
            }
            return;
        }

        Audio *SetVolume(float volume = 1.0f)
        {
            _volume = volume;
            MIX_SetTrackGain(_track, volume);
            return this;
        }
        Audio *SetPitch(float pitch = 1.0f)
        {
            _pitch = pitch;
            MIX_SetTrackFrequencyRatio(_track, _pitch);
            return this;
        }

        static MIX_Mixer *GetMixer()
        {
            return _mixer;
        }
    };
};
#endif