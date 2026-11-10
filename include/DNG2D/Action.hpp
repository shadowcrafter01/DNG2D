#ifndef DNG_ACTION_HPP
#define DNG_ACTION_HPP

#include <SDL3/SDL.h>
#include <vector>
#include <functional>
#include <optional>
#include "Input.hpp"

#define ACTION_WHILE_HELD 0
#define ACTION_ON_PRESS 1
#define ACTION_ON_RELEASE 2
#define ACTION_NOT_HELD 3

namespace DNG
{
    class Action
    {
    private:
        inline static std::vector<Action *> _instances = {};

        void _Update(std::unordered_set<SDL_Scancode> keys, std::unordered_set<Uint8> buttons)
        {
            _state = false;
            for (auto in : _inputs)
            {
                if (in.isMouseInput)
                {
                    if (buttons.count(in.button) > 0)
                    {
                        _state = true;
                        break;
                    }
                }
                else
                {
                    if (keys.count(in.key) > 0)
                    {
                        _state = true;
                        break;
                    }
                }
            }
            if (_state)
            {
                if (_flag_onPress)
                {
                    _flag_onPress = false;
                    for (auto f : _functions_true)
                    {
                        f();
                    }
                }
                for (auto f : _functions_while)
                {
                    f();
                }
            }
            else
            {
                _flag_onPress = true;
            }
            for (auto f : _functions_passKeyState)
            {
                f(_state);
            }
            if (!_state)
            {
                if (_flag_onRelease)
                {
                    _flag_onRelease = false;
                    for (auto f : _functions_false)
                    {
                        f();
                    }
                }
                for (auto f : _functions_not)
                {
                    f();
                }
            }
            else
            {
                _flag_onRelease = true;
            }
        }

        bool _flag_onPress = true;
        bool _flag_onRelease = true;

        std::vector<std::function<void(bool)>> _functions_passKeyState;
        std::vector<std::function<void()>> _functions_while;
        std::vector<std::function<void()>> _functions_not;
        std::vector<std::function<void()>> _functions_true;
        std::vector<std::function<void()>> _functions_false;

        struct InputType
        {
            InputType(SDL_Scancode keyboardKey)
            {
                isMouseInput = false;
                key = keyboardKey;
            }
            InputType(Uint8 mouseButton)
            {
                isMouseInput = true;
                button = mouseButton;
            }
            bool isMouseInput;
            SDL_Scancode key;
            Uint8 button;
        };

        std::vector<InputType> _inputs;

        bool _state = false;
        bool _active = true;

    public:
        Action()
        {
            _instances.push_back(this);
        }
        Action(std::function<void(bool)> f)
        {
            AssignFunction(f);
            _instances.push_back(this);
        }
        Action(std::function<void(bool)> f, SDL_Scancode key)
        {
            InputType newInput(key);
            _inputs.push_back(newInput);
            AssignFunction(f);
            _instances.push_back(this);
        }
        Action(std::function<void(bool)> f, Uint8 button)
        {
            InputType newInput(button);
            _inputs.push_back(button);
            AssignFunction(f);
            _instances.push_back(this);
        }
        Action(std::function<void()> f, int flag = ACTION_WHILE_HELD)
        {
            AssignFunction(f, flag);
            _instances.push_back(this);
        }
        Action(std::function<void()> f, SDL_Scancode key, int flag = ACTION_WHILE_HELD)
        {
            InputType newInput(key);
            _inputs.push_back(newInput);
            AssignFunction(f, flag);
            _instances.push_back(this);
        }
        Action(std::function<void()> f, Uint8 button, int flag = ACTION_WHILE_HELD)
        {
            InputType newInput(button);
            _inputs.push_back(button);
            AssignFunction(f, flag);
            _instances.push_back(this);
        }
        ~Action()
        {
        }

        void AssignFunction(std::function<void()> f, int flag = ACTION_WHILE_HELD)
        {
            switch (flag)
            {
            case ACTION_WHILE_HELD:
                _functions_while.push_back(f);
                break;
            case ACTION_ON_PRESS:
                _functions_true.push_back(f);
                break;
            case ACTION_ON_RELEASE:
                _functions_false.push_back(f);
                break;
            case ACTION_NOT_HELD:
                _functions_not.push_back(f);
                break;

            default:
                break;
            }
        }
        void AssignFunction(std::function<void(bool)> f)
        {
            _functions_passKeyState.push_back(f);
        }
        void AssignKey(SDL_Scancode key)
        {
            InputType newInput(key);
            _inputs.push_back(newInput);
        }
        void AssignKey(Uint8 button)
        {
            InputType newInput(button);
            _inputs.push_back(newInput);
        }

        bool GetState()
        {
            return _state;
        }

        Action *SetActive(bool state)
        {
            _active = state;
            return this;
        }

        static void UpdateAll()
        {
            std::unordered_set<SDL_Scancode> keys = Input::Keyboard::GetKeySet();
            std::unordered_set<Uint8> buttons = Input::Mouse::GetButtonSet();

            for (auto a : _instances)
            {
                if (a->_active)
                {
                    a->_Update(keys, buttons);
                }
            }
        }
    };
}

#endif