#ifndef DNG_MAIN_HPP
#define DNG_MAIN_HPP

#include <SDL3/SDL.h>
// #include <SDL3/SDL_main.h>
// #include <SDL3_image/SDL_image.h>

// #include <B2D/include/box2d/box2d.h>

#include <iostream>
#include <functional>

#include "DNG2D/Vector.hpp"

#include "DNG2D/Console.hpp"
#include "DNG2D/Window.hpp"
#include "DNG2D/Texture.hpp"
#include "DNG2D/Atlas.hpp"
#include "DNG2D/Font.hpp"
#include "DNG2D/Timer.hpp"
#include "DNG2D/Audio.hpp"
#include "DNG2D/Input.hpp"
#include "DNG2D/Camera.hpp"
#include "DNG2D/File.hpp"
#include "DNG2D/DrawTools.hpp"
#include "DNG2D/Pen.hpp"
#include "DNG2D/CollisionShape.hpp"
#include "DNG2D/Dingus.hpp"
#include "DNG2D/Math.hpp"
#include "DNG2D/Transform.hpp"
#include "DNG2D/Action.hpp"

namespace DNG
{


    inline static bool GAMESTATE = false;
    inline static Timer globalTimer;
    

    bool Init(const char *appname = "c++ project", const char *appversion = "0.0.0", const char *appidentifier = "com.name.Dngine")
    {
        bool flag = false;
        Console::LogLoadStart("Initializing engine");
        // metadata
        if (!SDL_SetAppMetadata(appname, appversion, appidentifier))
        {
            flag = true;
        }

        // init sdl
        if (!SDL_Init(SDL_INIT_VIDEO))
        {
            Console::LogLoadEnd(false, "Error initializing SDL3");
            return false;
        }

        if (flag)
        {
            Console::LogLoadEnd(true, (std::string) "Non-fatal errors encountered -> " + SDL_GetError());
        }
        else
        {
            Console::LogLoadEnd(true, "engine started!");
        }
        GAMESTATE = true;
        return true;
    }

    void Event()
    {
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            switch (e.type)
            {
            case SDL_EVENT_QUIT:
                GAMESTATE = false;
                break;
            case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
                GAMESTATE = false;
                break;
            default:
                break;
            }
            Input::Keyboard::Update(e);
            Input::Mouse::Update(e);
        }
    }

    bool Update()
    {
        globalTimer.Update();
        Event();
        Dingus::UpdateAll();
        Action::UpdateAll();
        Window::UpdateAll();
        return GAMESTATE;
    }

    void Shutdown()
    {
        Dingus::DestroyAll();
        b2DestroyWorld(Dingus::GetWorld());
        SDL_Quit();
        TTF_Quit();
    }

};

#endif