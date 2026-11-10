#ifndef DNG_WINDOW_HPP
#define DNG_WINDOW_HPP

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>
#include "DNG2D/Vector.hpp"
#include <vector>
#include "DNG2D/Console.hpp"
#include <string>

namespace DNG
{

    class Window
    {
    private:
        struct Renderer
        {
            SDL_Renderer *_pointer;
            Vector2<int> scale = {1, 1};
            operator SDL_Renderer *() const
            {
                return _pointer;
            }
        };

        //static std::vector<Window *> &instances()
        //{
        //    static std::vector<Window *> v;
        //    return v;
        //}
        inline static std::vector<Window *> _instances = {};

        SDL_Window *_pointer;
        Renderer _renderer;
        TTF_TextEngine *_ttfDngine;
        SDL_Texture *_cursor;
        Vector2<int> _cursorSize = Vector2<int>(0, 0);
        Vector2<float> _cursorScale = Vector2<int>(1, 1);
        bool flag_hasCustomCursor = false;
        const char *_title;
        Vector2<int> _size;
        Vector2<int> _center;
        SDL_WindowFlags _flags;

    public:
        Window(const char *title = "DNG2D", Vector2<int> size = Vector2<int>(640, 480), SDL_WindowFlags flags = SDL_WINDOW_RESIZABLE) : _title{title},
                                                                                                                                        _size{size},
                                                                                                                                        _flags{flags}
        {
            Console::LogLoadStart((std::string) "Creating window [" + _title + "]");
            if (!SDL_CreateWindowAndRenderer(_title, size.x, size.y, flags, &_pointer, &_renderer._pointer))
            {
                Console::LogLoadEnd(false);
                return;
            }
            if (!SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 0))
            {
                Console::LogLoadEnd(false);
                return;
            }
            _ttfDngine = TTF_CreateRendererTextEngine(_renderer);
            if (_ttfDngine == NULL)
            {
                Console::LogLoadEnd(false);
            }
            SDL_SetDefaultTextureScaleMode(_renderer, SDL_SCALEMODE_PIXELART);
            _instances.push_back(this);
            Console::LogLoadEnd(true);
        }
        ~Window()
        {
            if (_renderer._pointer)
                SDL_DestroyRenderer(_renderer);
            if (_pointer)
                SDL_DestroyWindow(_pointer);
            
            int i = 0;
            for (auto w : _instances)
            {
                if (this == w)
                {
                    _instances.erase(_instances.begin() + i);
                }
                i++;
            }
        }

        // Window(const Window&) = delete;
        // Window& operator=(const Window&) = delete;

        SDL_Window *GetPointer()
        {
            return _pointer;
        }
        Renderer GetRenderer()
        {
            return _renderer;
        }
        TTF_TextEngine *GetTextEngine()
        {
            return _ttfDngine;
        }
        Vector2<int> GetSize()
        {
            return _size;
        }
        Vector2<int> GetCenter()
        {
            return _center;
        }
        SDL_WindowFlags GetFlags()
        {
            return _flags;
        }

        Window *SetIcon(const char *path)
        {
            if (this == nullptr)
            {
                return nullptr;
            }

            SDL_Surface *surface = IMG_Load(path);
            if (surface == NULL)
            {
                Console::LogError((std::string) "Error loading texture file [" + path + "] for window [" + _title + "] icon");
                return nullptr;
            }
            int width = surface->w;
            int height = surface->h;
            if (!SDL_SetWindowIcon(_pointer, surface))
            {
                Console::LogError((std::string) "Error applying texture file [" + path + "] to window [" + _title + "] icon");
            }

            return this;
        }
        Window *SetCustomCursor(const char *path)
        {
            if (this == nullptr)
            {
                return nullptr;
            }

            SDL_Surface *surface = IMG_Load(path);
            _cursor = SDL_CreateTextureFromSurface(_renderer, surface);
            if (_cursor == NULL)
            {
                return nullptr;
            }
            _cursorSize = {surface->w, surface->h};
            if (!flag_hasCustomCursor)
            {
                SDL_HideCursor();
                flag_hasCustomCursor = true;
            }

            return this;
        }
        Window *SetCustomCursor(SDL_Texture *texture, Vector2<int> size)
        {
            if (this == nullptr)
            {
                return nullptr;
            }

            _cursor = texture;
            _cursorSize = size;
            if (!flag_hasCustomCursor)
            {
                SDL_HideCursor();
                flag_hasCustomCursor = true;
            }

            return this;
        }
        Window *SetCustomCursor(SDL_Surface *surface)
        {
            if (this == nullptr)
            {
                return nullptr;
            }

            _cursor = SDL_CreateTextureFromSurface(_renderer, surface);
            if (_cursor == NULL)
            {
                return nullptr;
            }
            _cursorSize = {surface->w, surface->h};
            if (!flag_hasCustomCursor)
            {
                SDL_HideCursor();
                flag_hasCustomCursor = true;
            }

            return this;
        }
        Window *SetCustomCursor()
        {
            if (this == nullptr)
            {
                return nullptr;
            }

            SDL_HideCursor();

            return this;
        }
        Window *SetCursorScale(Vector2<float> scale)
        {
            if (this == nullptr)
            {
                return nullptr;
            }

            _cursorScale = scale;

            return this;
        }
        Window *SetCursorScale(float scale = 1.0f)
        {
            if (this == nullptr)
            {
                return nullptr;
            }

            _cursorScale = Vector2(scale, scale);

            return this;
        }
        Window *SetWindowScale(int scale = 1.0f)
        {
            if (this == nullptr)
            {
                return nullptr;
            }

            _renderer.scale = Vector2<int>(scale, scale);
            SDL_SetRenderScale(_renderer, scale, scale);

            return this;
        }
        Window *SetWindowScale(Vector2<int> scale)
        {
            if (this == nullptr)
            {
                return nullptr;
            }

            _renderer.scale = scale;
            SDL_SetRenderScale(_renderer, scale.x, scale.y);

            return this;
        }

        void Update()
        {
            if (flag_hasCustomCursor)
            {
                float x, y;
                SDL_GetMouseState(&x, &y);
                SDL_FRect r;
                r.w = _cursorSize.x * _cursorScale.x;
                r.h = _cursorSize.y * _cursorScale.y;
                r.x = (x - ((r.w / 2) * _renderer.scale.x)) / _renderer.scale.x;
                r.y = (y - ((r.h / 2) * _renderer.scale.x)) / _renderer.scale.y;
                SDL_RenderTexture(_renderer, _cursor, NULL, &r);
            }
            SDL_SetRenderTarget(_renderer, NULL);
            SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 0);
            // SDL_SetRenderScale(renderer, 1, 1);
            SDL_GetWindowSizeInPixels(_pointer, &_size.x, &_size.y);
            _center = Vector2<int>(_size.x / 2, _size.y / 2);
            SDL_RenderPresent(_renderer);
            SDL_RenderClear(_renderer);
        }
        static void UpdateAll()
        {
            for (auto w : _instances)
            {
                w->Update();
            }
        }
        static std::vector<Window *> GetAllWindows()
        {
            return _instances;
        }
    };

};

#endif