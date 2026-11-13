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

        // static std::vector<Window *> &instances()
        //{
        //     static std::vector<Window *> v;
        //     return v;
        // }
        inline static std::vector<Window *> _instances = {};
        inline static int _activeWindows = 0;

        SDL_Window *_pointer;
        Renderer _renderer;
        TTF_TextEngine *_ttfEngine;
        SDL_Texture *_cursor;
        Vector2<int> _cursorSize = Vector2<int>(0, 0);
        Vector2<float> _cursorScale = Vector2<int>(1, 1);
        bool flag_hasCustomCursor = false;
        const char *_title;
        Vector2<int> _size;
        Vector2<int> _center;
        Vector2<int> _position;
        SDL_WindowFlags _flags;
        bool _state = true;

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
            _ttfEngine = TTF_CreateRendererTextEngine(_renderer);
            if (_ttfEngine == NULL)
            {
                Console::LogLoadEnd(false);
            }
            SDL_SetDefaultTextureScaleMode(_renderer, SDL_SCALEMODE_PIXELART);
            _instances.push_back(this);
            _activeWindows++;
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
                    _activeWindows--;
                }
                i++;
            }
            _pointer == nullptr;
        }

        /// @brief deletes and fully removes this window, careful with calling values from this window after removing it, run Hide() instead perchance?
        void Destroy()
        {
            delete this;
        }
        /// @brief similar to destroying a window but it doesnt actually delete anything, the window can be brought back with Show() (closing a window runs this)
        void Hide()
        {
            _state = false;
            _activeWindows--;
            SDL_HideWindow(_pointer);
        }
        void Show()
        {
            _state - true;
            _activeWindows++;
            SDL_ShowWindow(_pointer);
        }

        static void PurgeInactiveWindows()
        {
            for (auto w : _instances)
            {
                if (!w->GetState())
                    w->Destroy();
            }
        }


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
            return _ttfEngine;
        }
        Vector2<int> GetSize()
        {
            return _size;
        }
        Vector2<int> GetPosition()
        {
            return _position;
        }
        Vector2<int> GetCenter()
        {
            return _center;
        }
        SDL_WindowFlags GetFlags()
        {
            return _flags;
        }
        bool GetState()
        {
            return _state;
        }
        /// @brief gets all active windows
        /// @param force switch to include inactive windows as well
        /// @return returns a vector of all active window references
        static std::vector<Window *> GetAllWindows(bool force = false)
        {
            if (force)
            {
                return _instances;
            }
            else
            {
                std::vector<Window *> activeWindows = {};
                for (auto w : _instances)
                {
                    if (w->GetState())
                        activeWindows.push_back(w);
                }
                return activeWindows;
            }
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
            SDL_GetWindowPosition(_pointer, &_position.x, &_position.y);
            if (SDL_GetWindowFlags(_pointer) & SDL_WINDOW_INPUT_FOCUS)
            {
                if (flag_hasCustomCursor)
                {
                    SDL_HideCursor();
                    float x, y;
                    // SDL_GetMouseState(&x, &y);
                    SDL_GetGlobalMouseState(&x, &y);
                    SDL_FRect r;
                    r.w = _cursorSize.x * _cursorScale.x;
                    r.h = _cursorSize.y * _cursorScale.y;
                    r.x = ((x - ((r.w / 2) * _renderer.scale.x)) / _renderer.scale.x) - _position.x;
                    r.y = ((y - ((r.h / 2) * _renderer.scale.x)) / _renderer.scale.y) - _position.y;
                    SDL_RenderTexture(_renderer, _cursor, NULL, &r);
                }
                else
                {
                    SDL_ShowCursor();
                }
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
                if (w->GetState())
                    w->Update();
            }
        }
    };

};

#endif