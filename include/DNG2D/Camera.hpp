#ifndef DNG_CAMERA_HPP
#define DNG_CAMERA_HPP

#include "DNG2D/Window.hpp"
#include "DNG2D/Timer.hpp"

namespace DNG
{

    class Camera
    {
    private:
        Vector2<float> _position;
        float _zoom;
        float _angle;
        Window *_window;
    public:
        Camera(Window *window, Vector2<float> position = Vector2<float>(0, 0), float zoom = 1, float angle = 0) : _window{window},
                                                                                                                      _position{position},
                                                                                                                      _zoom{zoom},
                                                                                                                      _angle{angle}
        {
        }
        
        Vector2<float> GetPosition()
        {
            return _position;
        }
        float GetZoom()
        {
            return _zoom;
        } 
        float GetAngle()
        {
            return _angle;
        }
        Window *GetWindow()
        {
            return _window;
        }

        Camera *SetPosition(Vector2<float> position)
        {
            if (this == nullptr)
            {
                return nullptr;
            }
            _position = position;
            return this;
        }
        Camera *SetZoom(float zoom)
        {
            if (this == nullptr)
            {
                return nullptr;
            }
            _zoom = zoom;
            return this;
        } 
        Camera *SetAngle(float angle)
        {
            if (this == nullptr)
            {
                return nullptr;
            }
            _angle = angle;
            return this;
        }
        Camera *SetWindow(Window *window)
        {
            if (this == nullptr)
            {
                return nullptr;
            }
            _window = window;
            return this;
        }

        Camera *ChangePositionBy(Vector2<float> dxy, double delta = 1.0)
        {
            if (this == nullptr)
            {
                return nullptr;
            }
            _position += dxy * delta;
            return this;
        }
        Camera *ChangePositionBy(float dx = 0.0f, float dy = 0.0f, double delta = 1.0)
        {
            if (this == nullptr)
            {
                return nullptr;
            }
            _position.x += dx * delta;
            _position.y += dy * delta;
            return this;
        }
        Camera *ChangeZoomBy(float dz, double delta = 1.0)
        {
            if (this == nullptr)
            {
                return nullptr;
            }
            _zoom += dz * delta;
            return this;
        }
        Camera *ChangeAngleBy(float da, double delta = 1.0)
        {
            if (this == nullptr)
            {
                return nullptr;
            }
            _angle += da * delta;
            return this;
        }

    };
};
#endif