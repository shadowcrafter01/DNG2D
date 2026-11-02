#ifndef ENG_MAIN_HPP
#define ENG_MAIN_HPP

#include <SDL3/SDL.h>
// #include <SDL3/SDL_main.h>
// #include <SDL3_image/SDL_image.h>

// #include <B2D/include/box2d/box2d.h>

#include <iostream>
#include <functional>

#include "ENG2D/Vector.hpp"

#include "ENG2D/Console.hpp"
#include "ENG2D/Window.hpp"
#include "ENG2D/Texture.hpp"
#include "ENG2D/Atlas.hpp"
#include "ENG2D/Font.hpp"
#include "ENG2D/Timer.hpp"
#include "ENG2D/Audio.hpp"
#include "ENG2D/Input.hpp"
#include "ENG2D/Camera.hpp"
#include "ENG2D/File.hpp"
#include "ENG2D/DrawTools.hpp"
#include "ENG2D/Pen.hpp"
#include "ENG2D/CollisionShape.hpp"
#include "ENG2D/Dingus.hpp"
#include "ENG2D/Math.hpp"
#include "ENG2D/Transform.hpp"
#include "ENG2D/Action.hpp"

namespace ENG
{


    inline static bool GAMESTATE = false;
    inline static Timer globalTimer;
    

    bool Init(const char *appname = "c++ project", const char *appversion = "0.0.0", const char *appidentifier = "com.name.engine")
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
            Console::LogLoadEnd(true, "Engine started!");
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