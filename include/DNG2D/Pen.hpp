#ifndef DNG_PEN_HPP
#define DNG_PEN_HPP

#include <DNG2D/Camera.hpp>
#include <DNG2D/DrawTools.hpp>
#include "Vector.hpp"

namespace DNG
{

    class Pen
    {
    private:
    public:
        Pen(Camera *camera) : camera{camera}
        {
        }

        void Down()
        {
            isDown = true;
        }
        void Up()
        {
            isDown = false;
        }
        void GoTo(Vector2<double> newPos)
        {
            if (isDown)
            {
                DrawTools::DrawLine(camera, pos, newPos);
            }
            pos = newPos;
        }

        Vector2<double> pos = {0, 0};
        bool isDown = false;
        Camera *camera;
    };

};

#endif