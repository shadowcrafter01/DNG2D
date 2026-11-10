#ifndef DNG_ATLAS_HPP
#define DNG_ATLAS_HPP

#include "DNG2D/Texture.hpp"

namespace DNG
{
    class Atlas
    {
    private:
    
        Texture *_texture;
        SDL_FRect _rect;
    
    public:
        Atlas(Window *window, const char *path)
        {
            Texture texture = Texture(window, path);
            _texture = &texture;

            Console::LogInfo((std::string) "Assigned Texture [" + path + "] to atlas", " -LOAD : ");
        }
        Atlas(Texture *texture)
        {
            _texture = texture;
            Console::LogInfo((std::string) "Assigned Texture [" + texture->GetFilePath() + "] to atlas", " -LOAD : ");
        }
        ~Atlas()
        {
        }

        static Atlas Create(Window *window, const char *path)
        {
            Atlas atlas(window, path);
            return atlas;
        }
        static Atlas Create(Texture *texture)
        {
            Atlas atlas(texture);
            return atlas;
        }

        SDL_FRect GetRect()
        {
            return _rect;
        }
        Texture *GetTexture()
        {
            return _texture;
        }
        
    };
};
#endif