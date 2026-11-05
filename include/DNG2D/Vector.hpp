#ifndef DNG_VECTOR2_HPP
#define DNG_VECTOR2_HPP

#include <math.h>
#include <string>
#include <sstream>
#include <box2d/box2d.h>

namespace DNG
{

    template <class T = float>
    class Vector2
    {
    private:
    public:
        Vector2(const T &X = 0, const T &Y = 0) : x{X}, y{Y}
        {
        }
        Vector2(b2Vec2 vec) : x{vec.x}, y{vec.y}
        {
        }

        T x;
        T y;

        Vector2<T> Norm(bool return_only = true)
        {
            T abs = this->Magnitude();
            Vector2<T> out = {x / abs, y / abs};
            if (!return_only)
            {
                x = out.x;
                y = out.y;
            }
            return out;
        }
        T Magnitude(bool return_only = true)
        {
            return sqrt((x * x) + (y * y));
        }
        Vector2<T> Rotate(double angle, bool return_only = false)
        {
            T _x = (cos((0.017453293) * (-angle)) * x) + (sin((0.017453293) * (-angle)) * y);
            T _y = (-sin((0.017453293) * (-angle)) * x) + (cos((0.017453293) * (-angle)) * y);
            if (!return_only)
            {
                x = _x;
                y = _y;
            }
            return Vector2<T>(_x, _y);
        }
        Vector2<T> Scale(T factor, bool return_only = false)
        {
            T _x = x * factor;
            T _y = y * factor;
            if (!return_only)
            {
                x = _x;
                y = _y;
            }
            return Vector2<T>(_x, _y);
        }

        inline static T Distance(Vector2<T> vec1, Vector2<T> vec2)
        {
            T xDiff = vec1.x - vec2.x;
            T yDiff = vec1.y - vec2.y;
            return sqrt((xDiff * xDiff) + (yDiff * yDiff));
        }
        inline static T Angle(Vector2<T> vec1, Vector2<T> vec2)
        {
            return atan2(vec2.y - vec1.y, vec2.x - vec1.x);
        }
        inline static T Normalized(Vector2<T> vec)
        {
            T abs = vec.Magnitude();
            return Vector2(vec.x / abs, vec.y / abs);
        }
        inline static Vector2<T> B2D_to_DNG(b2Vec2 vec)
        {
            return (Vector2<T>){(T)vec.x, (T)vec.y};
        }

        template <class U>
        Vector2<T> operator+(const Vector2<U> &r)
        {
            return Vector2<T>(x + r.x, y + r.y);
        }
        template <class U>
        Vector2<T> operator+=(const Vector2<U> &r)
        {
            x += r.x;
            y += r.y;
            return *this;
        }
        template <class U>
        Vector2<T> operator-(const Vector2<U> &r)
        {
            return Vector2<T>(x - r.x, y - r.y);
        }
        template <class U>
        Vector2<T> operator-=(const Vector2<U> &r)
        {
            x -= r.x;
            y -= r.y;
            return *this;
        }
        template <class U>
        Vector2<T> operator*(const Vector2<U> &r)
        {
            return Vector2<T>(x * r.x, y * r.y);
        }
        template <class U>
        Vector2<T> operator*=(const Vector2<U> &r)
        {
            x *= r.x;
            y *= r.y;
            return *this;
        }
        template <class U>
        Vector2<T> operator/(const Vector2<U> &r)
        {
            return Vector2(x / r.x, y / r.y);
        }
        template <class U>
        Vector2<T> operator/=(const Vector2<U> &r)
        {
            x /= r.x;
            y /= r.y;
            return *this;
        }

        template <class U>
        Vector2<T> operator+(const U &r)
        {
            return Vector2<T>(x + r, y + r);
        }
        template <class U>
        Vector2<T> operator+=(const U &r)
        {
            x += r;
            y += r;
            return *this;
        }
        template <class U>
        Vector2<T> operator-(const U &r)
        {
            return Vector2<T>(x - r, y - r);
        }
        template <class U>
        Vector2<T> operator-=(const U &r)
        {
            x -= r;
            y -= r;
            return *this;
        }
        template <class U>
        Vector2<T> operator*(const U &r)
        {
            Vector2<T> out = {x * (T)r, y * (T)r};
            return out;
        }
        template <class U>
        Vector2<T> operator*=(const U &r)
        {
            x *= (T)r;
            y *= (T)r;
            return *this;
        }
        template <class U>
        Vector2<T> operator/(const U &r)
        {
            return Vector2<T>(x / (T)r, y / (T)r);
        }
        template <class U>
        Vector2<T> operator/=(const U &r)
        {
            x /= (T)r;
            y /= (T)r;
            return *this;
        }
        template <class U>
        bool operator==(const Vector2<U> &r)
        {
            if (x == r.x || y == r.y)
            {
                return true;
            }
            return false;
        }
        template <class U>
        bool operator!=(const Vector2<U> &r)
        {
            if (x != r.x || y != r.y)
            {
                return true;
            }
            return false;
        }

        operator Vector2<double>()
        {
            Vector2<double> r = Vector2<double>((double)x, (double)y);
            return r;
        }
        operator Vector2<int>()
        {
            Vector2<int> r = Vector2<int>((int)x, (int)y);
            return r;
        }
        operator Vector2<float>()
        {
            Vector2<float> r = Vector2<float>((float)x, (float)y);
            return r;
        }
        operator std::string()
        {
            std::stringstream Xout;
            Xout << x;
            std::stringstream Yout;
            Yout << y;

            return "(" + Xout.str() + "," + Yout.str() + ")";
        }
        operator const char *()
        {
            std::stringstream Xout;
            Xout << x;
            std::stringstream Yout;
            Yout << y;

            std::string out = "(" + Xout.str() + "," + Yout.str() + ")";

            return out.data();
        }
        operator b2Vec2()
        {
            return (b2Vec2){x, y};
        }
    };

};

#endif