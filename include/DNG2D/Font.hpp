#ifndef DNG_FONT_HPP
#define DNG_FONT_HPP

#include <SDL3_ttf/SDL_ttf.h>
#include <string>

#include "DNG2D/Console.hpp"
#include "DNG2D/Texture.hpp"

namespace DNG
{

    class Font
    {
    private:
        static inline bool flag_ttfInit = false;

        TTF_Font *font;
        std::string path;
        int point;
        bool state = false;

    public:
        Font(std::string path, int point) : path{path},
                                            point{point}
        {
            if (!flag_ttfInit)
            {
                Console::LogLoadStart("Initializing SDL_TTF");
                if (!TTF_Init())
                {
                    Console::LogLoadEnd(false);
                    return;
                }
                Console::LogLoadEnd(true);
                flag_ttfInit = true;
            }
            Console::LogLoadStart("Loading TTF font [" + path + "]");
            font = TTF_OpenFont(path.c_str(), point);
            if (font == NULL)
            {
                Console::LogLoadEnd(false);
                return;
            }
            Console::LogLoadEnd(true);
            state = true;
        }
        TTF_Text *MakeText(std::string text, Window *window)
        {
            TTF_Text *textOut = TTF_CreateText(window->GetTextEngine(), font, text.c_str(), 0);
            TTF_SetTextColor(textOut, 255, 255, 255, SDL_ALPHA_OPAQUE);
            return textOut;
        }
        
        TTF_Font *GetFont()
        {
            return font;
        }
        std::string GetPath()
        {
            return path;
        }
        int GetPoint()
        {
            return point;
        }
        bool GetState()
        {
            return state;
        }

    };

};

#endif