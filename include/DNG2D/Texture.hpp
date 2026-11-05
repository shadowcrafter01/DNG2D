#ifndef DNG_TEXTURE_HPP
#define DNG_TEXTURE_HPP

#include "DNG2D/Window.hpp"
#include "DNG2D/Vector.hpp"
#include "DNG2D/Console.hpp"
#include <SDL3_image/SDL_image.h>
#include "DNG2D/Font.hpp"
#include "DNG2D/Camera.hpp"

namespace DNG
{

    class Texture
    {
    private:
        Window *_window;
        const char *_path;
        SDL_Texture *_pointer;
        SDL_Surface *_surface;
        bool _state = false;
        Vector2<int> _size;

    public:
        Texture(Window *window, const char *path) : _path{path},
                                                    _window{window}
        {
            Console::LogLoadStart((std::string) "Loading texture [" + path + "]");

            _surface = IMG_Load(path);
            if (_surface == NULL)
            {
                Console::LogLoadEnd(false);
                return;
            }
            _size = Vector2<int>(_surface->w, _surface->h);
            _pointer = SDL_CreateTextureFromSurface(window->GetRenderer(), _surface);
            if (_pointer == NULL)
            {
                Console::LogLoadEnd(false);
                return;
            }
            Console::LogLoadEnd(true);
            _state = true;
        }
        
        Window *GetWindow()
        {
            return _window;
        }
        const char *GetFilePath()
        {
            return _path;
        }
        SDL_Texture *GetPointer()
        {
            return _pointer;
        }
        SDL_Surface *GetSurface()
        {
            return _surface;
        }
        bool GetState()
        {
            return _state;
        }
        Vector2<int> GetSize()
        {
            return _size;
        }

        operator SDL_Texture *()
        {
            return _pointer;
        }
        operator SDL_Surface *()
        {
            return _surface;
        }
        template <typename T>
        operator Vector2<T>()
        {
            return Vector2<T>((T)_size.x, (T)_size.y);
        }
    };

};

#endif