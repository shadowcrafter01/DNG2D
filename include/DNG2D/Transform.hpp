#ifndef DNG_TRANSFORM2_HPP
#define DNG_TRANSFORM2_HPP

#include "DNG2D/Texture.hpp"
#include <box2d/box2d.h>
#include "DNG2D/Math.hpp"

namespace DNG
{
    template <class T = float>
    class Transform2
    {
    private:
        /* data */
    public:
        Transform2(T x = 0, T y = 0, T w = 1, T h = 1, T a = 0) : position{Vector2<T>(x, y)},
                                                                  size{Vector2<T>(w, h)},
                                                                  angle{a}
        {
        }
        Transform2(Vector2<T> pos, Vector2<T> size, T angle = 0) : position{pos},
                                                                   size{size},
                                                                   angle{angle}
        {
        }
        Transform2(Vector2<T> pos, T w, T h, T angle = 0) : position{pos},
                                                            size{Vector2<T>(w, h)},
                                                            angle{angle}
        {
        }
        Transform2(T x, T y, Vector2<T> size, T angle = 0) : position{Vector2<T>(x, y)},
                                                             size{size},
                                                             angle{angle}
        {
        }

        bool ContainsPoint(Vector2<T> point)
        {
            Vector2<T> dv = point - position;
            dv.Rotate(angle);

            return (dv.x <= (size.x / (T)2.0) && dv.x >= -(size.x / (T)2.0)) && (dv.y <= (size.y / (T)2.0) && dv.y >= -(size.y / (T)2.0));
        }

        Vector2<T> position;
        Vector2<T> size;
        T angle;

        operator b2Rot()
        {
            return b2MakeRot(Math::deg2rad(angle));
        }
        operator b2Vec2()
        {
            return position;
        }
    };
};
#endif