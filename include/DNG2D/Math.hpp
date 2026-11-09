#ifndef DNG_MATH_HPP
#define DNG_MATH_HPP

#include <cmath>

namespace DNG
{

    class Math
    {
    public:
        template <typename T>
        static T Lerp(T a, T b, float t)
        {
            return (a * (1 - t)) + (b * t);
        }

        template <typename T>
        static T rad2deg(T rad)
        {
            return rad * 180.0 / M_PI;
        }

        template <typename T>
        static T deg2rad(T deg)
        {
            return deg * M_PI / 180.0;
        }

        template <typename T>
        static T Clamp(T value, T min, T max)
        {
            if (value > max)
            {
                return max;
            }
            if (value < min)
            {
                return min;
            }
            return value;
        }
        template <typename T>
        static void Clamp(T *value, T min, T max)
        {
            if (value > max)
            {
                value = max;
            }
            if (value < min)
            {
                value = min;
            }
        }
    };

};

#endif