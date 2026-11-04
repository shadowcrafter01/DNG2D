#ifndef ENG_ANIMATEDTEXTURE_HPP
#define ENG_ANIMATEDTEXTURE_HPP

#include "ENG2D/Atlas.hpp"
#include <vector>

namespace ENG
{

    class AnimatedTexture
    {
    private:
        int _frameCount;
        Atlas *_atlas;
        double _FPS;
        int _frameWidth;
        int _currentFrame = 0;

    public:
        AnimatedTexture(Atlas *atlas, int frame_width, double framerate) : _frameWidth{frame_width},
                                                                           _atlas{atlas}
        {
            _frameCount = atlas->GetTexture()->GetSize().x / frame_width;
        }

        int GetFrameCount()
        {
            return _frameCount;
        }
        Atlas *GetAtlas()
        {
            return _atlas;
        }
        double GetFPS()
        {
            return _FPS;
        }
        int GetFrameWidth()
        {
            return _frameWidth;
        }
        int GetCurrentFrame()
        {
            return _currentFrame;
        }
        Vector2<int> GetSize()
        {
            return Vector2<int>(_frameWidth, _atlas->GetTexture()->GetSize().y);
        }
    };
};
#endif