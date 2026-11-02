#ifndef ENG_INPUT_CPP
#define ENG_INPUT_CPP

#include <SDL3/SDL.h>
#include <ENG2D/Vector.hpp>
// #include <unordered_map>
#include <unordered_set>
#include <functional>
#include <vector>

#include "ENG2D/Camera.hpp"

namespace ENG::Input
{

    class Mouse
    {
    private:
        // mouse state
        // inline static std::unordered_set<Uint8> _mouseButtons;

        inline static float _mouseX = 0.0f;
        inline static float _mouseY = 0.0f;
        inline static float _mouseX_fenced = 0.0f;
        inline static float _mouseY_fenced = 0.0f;
        inline static float _mouseU = 0.0f;
        inline static float _mouseV = 0.0f;
        inline static float _wheelX = 0.0f;
        inline static float _wheelY = 0.0f;

        inline static std::vector<std::function<void()>> _mouseDownFunctionsR; // right click
        inline static std::vector<std::function<void()>> _mouseDownFunctionsM; // middle click
        inline static std::vector<std::function<void()>> _mouseDownFunctionsL; // left click

        inline static std::vector<std::function<void()>> _mouseUpFunctionsR; // right click
        inline static std::vector<std::function<void()>> _mouseUpFunctionsM; // middle click
        inline static std::vector<std::function<void()>> _mouseUpFunctionsL; // left click

        inline static std::unordered_set<Uint8> _buttons;

    public:
        static void Update(const SDL_Event &event)
        {
            switch (event.type)
            {
            case SDL_EVENT_MOUSE_MOTION:
                int w;
                int h;
                SDL_GetWindowSize(SDL_GetWindowFromID(event.window.windowID), &w, &h);
                _mouseX = event.motion.x;
                _mouseY = event.motion.y;
                _mouseX_fenced = SDL_clamp(event.motion.x, 0, w);
                _mouseY_fenced = SDL_clamp(event.motion.y, 0, h);
                _mouseU += event.motion.xrel;
                _mouseV += event.motion.yrel;
                break;
            case SDL_EVENT_MOUSE_BUTTON_DOWN:
                _buttons.insert(event.button.button);

                switch (event.button.button)
                {
                case SDL_BUTTON_LEFT:
                    Left = true;
                    for (std::function<void()> &func : _mouseDownFunctionsL)
                    {
                        func();
                    }
                    break;
                case SDL_BUTTON_RIGHT:
                    Right = true;
                    for (std::function<void()> &func : _mouseDownFunctionsR)
                    {
                        func();
                    }
                    break;
                case SDL_BUTTON_MIDDLE:
                    Middle = true;
                    for (std::function<void()> &func : _mouseDownFunctionsM)
                    {
                        func();
                    }
                    break;

                default:
                    break;
                }

                break;
            case SDL_EVENT_MOUSE_BUTTON_UP:
                _buttons.erase(event.button.button);

                switch (event.button.button)
                {
                case SDL_BUTTON_LEFT:
                    Left = false;
                    for (std::function<void()> &func : _mouseUpFunctionsL)
                    {
                        func();
                    }
                    break;
                case SDL_BUTTON_RIGHT:
                    Right = false;
                    for (std::function<void()> &func : _mouseUpFunctionsR)
                    {
                        func();
                    }
                    break;
                case SDL_BUTTON_MIDDLE:
                    Middle = false;
                    for (std::function<void()> &func : _mouseUpFunctionsM)
                    {
                        func();
                    }
                    break;

                default:
                    break;
                }

                break;
            case SDL_EVENT_MOUSE_WHEEL:
                _wheelX += event.wheel.x;
                _wheelY += event.wheel.y;
                break;

            default:
                break;
            }
        }

        // Assign functions to mouse inputs
        inline static void AssignMouseDown_R(std::function<void()> function)
        {
            _mouseDownFunctionsR.push_back(function);
        }
        inline static void AssignMouseDown_L(std::function<void()> function)
        {
            _mouseDownFunctionsL.push_back(function);
        }
        inline static void AssignMouseDown_M(std::function<void()> function)
        {
            _mouseDownFunctionsM.push_back(function);
        }
        inline static void AssignMouseUp_R(std::function<void()> function)
        {
            _mouseUpFunctionsR.push_back(function);
        }
        inline static void AssignMouseUp_L(std::function<void()> function)
        {
            _mouseUpFunctionsL.push_back(function);
        }
        inline static void AssignMouseUp_M(std::function<void()> function)
        {
            _mouseUpFunctionsM.push_back(function);
        }

        // raw mouse state (for getting inputs from random buttons besides R L or M)
        static bool GetMouseState(Uint8 button)
        {
            return SDL_GetMouseState(NULL, NULL) == button;
        }
        inline static bool Left = false;
        inline static bool Right = false;
        inline static bool Middle = false;

        static Vector2<float> GetMousePos(bool fenced = false)
        {
            if (fenced)
            {
                return Vector2<float>(_mouseX_fenced, _mouseY_fenced);
            }
            else
            {
                return Vector2<float>(_mouseX, _mouseY);
            }
        }
        static Vector2<float> GetMouseWorldPos(Camera *camera, bool fenced = false)
        {
            float globalX;
            float globalY;
            SDL_GetGlobalMouseState(&globalX, &globalY);

            int winX;
            int winY;
            SDL_GetWindowPosition(camera->GetWindow()->GetPointer(), &winX, &winY);

            float outX = globalX - static_cast<float>(winX);
            float outY = globalY - static_cast<float>(winY);

            Vector2 out(outX, outY);

            out.x -= camera->GetWindow()->GetCenter().x;
            out.y -= camera->GetWindow()->GetCenter().y;

            out.y *= -1;

            out.Rotate(camera->GetAngle());

            out /= camera->GetZoom();

            out /= camera->GetWindow()->GetRenderer().scale;

            out.x += camera->GetPosition().x;
            out.y += camera->GetPosition().y;

            return out;
        }
        static Vector2<float> GetMouseVel()
        {
            return Vector2<float>(_mouseU, _mouseV);
        }
        static std::unordered_set<Uint8> GetButtonSet()
        {
            return _buttons;
        }
    };

    class Keyboard
    {
    private:
        inline static std::unordered_set<SDL_Scancode> _keys;

    public:
        static void Update(const SDL_Event &event)
        {
            switch (event.type)
            {
            case SDL_EVENT_KEY_DOWN:
                if (!event.key.repeat)
                {
                    _keys.insert(event.key.scancode);
                }
                break;
            case SDL_EVENT_KEY_UP:
                _keys.erase(event.key.scancode);
                break;
            default:
                break;
            }
        }

        // keyboard stuf
        static bool KeyState(SDL_Scancode sc)
        {
            return _keys.count(sc) != 0;
        }
        static std::unordered_set<SDL_Scancode> GetKeySet()
        {
            return _keys;
        }
    };

};

#endif