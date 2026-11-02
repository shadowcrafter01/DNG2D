#ifndef ENG_AUDIO_HPP
#define ENG_AUDIO_HPP

#include <SDL3_mixer/SDL_mixer.h>

// #include "Main.hpp"
#include "ENG2D/Console.hpp"

namespace ENG
{

    class Audio
    {
    private:
        static inline MIX_Mixer *_mixer;
        static inline bool _flag_mixerInit = false;

        MIX_Audio *_audio;
        bool _state = false;

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
            Console::LogLoadEnd(true);
            _state = true;
        }

        void Play()
        {
            if (!MIX_PlayAudio(_mixer, _audio))
            {
                Console::LogError("Play Audio Fail");
            }
        }
        static void Play(Audio *audio)
        {
            audio->Play();
        }

        
    };
};
#endif