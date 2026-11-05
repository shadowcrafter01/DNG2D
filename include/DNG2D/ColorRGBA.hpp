#ifndef DNG_COLORRGBA_H
#define DNG_COLORRGBA_H

#include <SDL3/SDL.h>

namespace DNG
{

    class ColorRGBA
    {
    private:

        bool clamp(Uint8 *value, Uint8 min, Uint8 max)
        {
            if (*value > max)
            {
                *value = max;
                return true;
            }
            if (*value < min)
            {
                *value = min;
                return true;
            }
            return false;
        }

    public:
        ColorRGBA(Uint8 r = 255, Uint8 g = 255, Uint8 b = 255, Uint8 a = 255) : red{r},
                                                                  green{g},
                                                                  blue{b},
                                                                  alpha{a}
        {
            clamp(&red, 0, 255);
            clamp(&green, 0, 255);
            clamp(&blue, 0, 255);
            clamp(&alpha, 0, 255);
        }
        Uint8 red;
        Uint8 green;
        Uint8 blue;
        Uint8 alpha;

        operator SDL_Color()
        {
            SDL_Color out;
            out.r = red;
            out.g = green;
            out.b = blue;
            out.a = alpha;
            return out;
        }
    };

};

#define RGBA_TEXTURE_UNCHANGED ColorRGBA(255, 255, 255, 255)
#define RGBA_BLACK ColorRGBA(0, 0, 0, 255)
#define RGBA_WHITE ColorRGBA(255, 255, 255, 0)
#define RGBA_RED ColorRGBA(255, 0, 0, 255)
#define RGBA_ORANGE ColorRGBA(255, 64, 64, 255)
#define RGBA_YELLOW ColorRGBA(255, 255, 0, 255)
#define RGBA_GREEN ColorRGBA(0, 255, 0, 255)
#define RGBA_BLUE ColorRGBA(0, 0, 255, 255)
#define RGBA_PURPLE ColorRGBA(255, 0, 255, 255)

#endif