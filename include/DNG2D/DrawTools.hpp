#ifndef DNG_DRAWTOOLS_HPP
#define DNG_DRAWTOOLS_HPP

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include "DNG2D/Camera.hpp"
#include "DNG2D/Texture.hpp"
#include "DNG2D/Atlas.hpp"
#include "DNG2D/AnimatedTexture.hpp"
#include "DNG2D/Font.hpp"
#include <cmath>
#include "DNG2D/Vector.hpp"
#include "DNG2D/ColorRGBA.hpp"

namespace DNG
{

    class DrawTools
    {
    private:
        static Vector2<float> projectToCamera(Camera *camera, Vector2<float> vector)
        {
            Vector2<float> out = vector;

            // offset by camera pos
            out.x -= camera->GetPosition().x;
            out.y -= camera->GetPosition().y;

            // scale by zoom
            if (camera->GetZoom() != 1)
                out *= camera->GetZoom();

            // rotate by camera angle
            out.Rotate(-camera->GetAngle());

            // flip y axis
            out.y = -out.y;

            // center on window
            out.x += camera->GetWindow()->GetCenter().x / camera->GetWindow()->GetRenderer().scale.x;
            out.y += camera->GetWindow()->GetCenter().y / camera->GetWindow()->GetRenderer().scale.y;

            // scale by renderer scale
            // if (camera->window->renderer.scale != Vector2<int>(1,1)) out /= camera->window->renderer.scale;

            return out;
        }
        static bool onscreen(Camera *camera, SDL_FRect *rect)
        {
            float size = std::max(rect->w, rect->h);

            if (rect->x + size < 0 || rect->y + size < 0 || rect->x - size > camera->GetWindow()->GetSize().x || rect->y - size > camera->GetWindow()->GetSize().y)
            {
                return false;
            }
            return true;
        }

    public:
        static void DrawTexture(Camera *camera, Texture *texture, Vector2<float> position, Vector2<float> size = Vector2<float>(1.0f, 1.0f), float angle = 0, ColorRGBA color = ColorRGBA(255, 255, 255, 255))
        {

            Vector2<float> pos = projectToCamera(camera, position);
            SDL_FRect r;
            r.w = size.x * camera->GetZoom();
            r.h = size.y * camera->GetZoom();
            r.x = pos.x - (r.w / 2);
            r.y = pos.y - (r.h / 2);

            if (onscreen(camera, &r))
            {
                // texture->pointer = SDL_CreateTextureFromSurface(camera->window->renderer, texture->surface);
                SDL_SetTextureBlendMode(texture->GetPointer(), SDL_BLENDMODE_BLEND);
                SDL_SetTextureColorMod(texture->GetPointer(), color.red, color.green, color.blue);
                SDL_SetTextureAlphaMod(texture->GetPointer(), color.alpha);
                SDL_RenderTextureRotated(texture->GetWindow()->GetRenderer(), texture->GetPointer(), NULL, &r, angle + camera->GetAngle(), NULL, SDL_FLIP_NONE);
                // SDL_DestroyTexture(texture->pointer);
            }
        }
        static void DrawTexture(Texture *texture, Vector2<float> position, Vector2<float> size = Vector2<float>(1.0f, 1.0f), float angle = 0, ColorRGBA color = ColorRGBA(255, 255, 255, 255))
        {

            SDL_FRect r;
            r.w = size.x;
            r.h = size.y;
            r.x = position.x - (r.w / 2);
            r.y = position.y - (r.h / 2);
            Vector2<float> vr(r.x, r.y);

            SDL_SetTextureBlendMode(texture->GetPointer(), SDL_BLENDMODE_BLEND);
            SDL_SetTextureColorMod(texture->GetPointer(), color.red, color.green, color.blue);
            SDL_SetTextureAlphaMod(texture->GetPointer(), color.alpha);
            SDL_RenderTextureRotated(texture->GetWindow()->GetRenderer(), texture->GetPointer(), NULL, &r, angle, NULL, SDL_FLIP_NONE);
        }

        static void DrawAtlas(Camera *camera, Atlas *atlas, Vector2<float> position, Vector2<float> size = Vector2<float>(1.0f, 1.0f), float angle = 0, ColorRGBA color = ColorRGBA(255, 255, 255, 255))
        {

            Vector2<float> pos = projectToCamera(camera, position);
            SDL_FRect r;
            r.h = size.x * camera->GetZoom();
            r.w = size.y * camera->GetZoom();
            r.x = pos.x - (r.w / 2);
            r.y = pos.y - (r.h / 2);
            Vector2<float> vr(r.x, r.y);

            if (onscreen(camera, &r))
            {
                SDL_FRect s;
                s.h = atlas->GetRect().h;
                s.w = atlas->GetRect().w;
                s.x = atlas->GetRect().x + (atlas->GetTexture()->GetSize().x / 2) - (s.w / 2);
                s.y = -atlas->GetRect().y + (atlas->GetTexture()->GetSize().y / 2) - (s.h / 2);

                SDL_SetTextureBlendMode(atlas->GetTexture()->GetPointer(), SDL_BLENDMODE_BLEND);
                SDL_SetTextureColorMod(atlas->GetTexture()->GetPointer(), color.red, color.green, color.blue);
                SDL_SetTextureAlphaMod(atlas->GetTexture()->GetPointer(), color.alpha);
                SDL_RenderTextureRotated(atlas->GetTexture()->GetWindow()->GetRenderer(), atlas->GetTexture()->GetPointer(), &s, &r, angle + camera->GetAngle(), NULL, SDL_FLIP_NONE);
            }
        }

        static void DrawAnimatedTexture(Camera *camera, AnimatedTexture *animatedTexture, Vector2<float> position, int frame = -1, Vector2<float> size = Vector2<float>(1.0, 1.0), float angle = 0, ColorRGBA color = ColorRGBA(255, 255, 255, 255))
        {
            if (frame == -1)
            {
                frame = animatedTexture->GetCurrentFrame();
            }
            else
            {
                frame = frame % animatedTexture->GetFrameCount();
            }

            Vector2<float> pos = projectToCamera(camera, position);
            SDL_FRect r;
            r.w = size.x * camera->GetZoom();
            r.h = size.y * camera->GetZoom();
            r.x = pos.x - (r.w / 2);
            r.y = pos.y - (r.h / 2);

            if (onscreen(camera, &r))
            {
                SDL_FRect s;
                s.w = animatedTexture->GetFrameWidth();
                s.h = animatedTexture->GetAtlas()->GetTexture()->GetSize().y;
                s.x = animatedTexture->GetFrameWidth() * frame;
                s.y = 0;

                SDL_SetTextureBlendMode(animatedTexture->GetAtlas()->GetTexture()->GetPointer(), SDL_BLENDMODE_BLEND);
                SDL_SetTextureColorMod(animatedTexture->GetAtlas()->GetTexture()->GetPointer(), color.red, color.green, color.blue);
                SDL_SetTextureAlphaMod(animatedTexture->GetAtlas()->GetTexture()->GetPointer(), color.alpha);
                SDL_RenderTextureRotated(animatedTexture->GetAtlas()->GetTexture()->GetWindow()->GetRenderer(), animatedTexture->GetAtlas()->GetTexture()->GetPointer(), &s, &r, angle + camera->GetAngle(), NULL, SDL_FLIP_NONE);
            }
        }

        static void DrawFont(Camera *camera, Font *font, std::string message, Vector2<float> position, float size = 1, float angle = 0, ColorRGBA color = ColorRGBA(255, 255, 255, 255), Vector2<float> scale = Vector2<float>(1, 1))
        {
            if (message == "" || !font->GetState())
            {
                return;
            }

            SDL_Surface *textSurface = TTF_RenderText_Solid(font->GetFont(), message.c_str(), 0, color);
            SDL_Texture *texture = SDL_CreateTextureFromSurface(camera->GetWindow()->GetRenderer(), textSurface);
            Vector2<float> pos = projectToCamera(camera, position);
            SDL_FRect r;
            r.h = size * scale.x * camera->GetZoom() * textSurface->h;
            r.w = size * scale.y * camera->GetZoom() * textSurface->w;
            r.x = pos.x;
            r.y = pos.y;

            if (onscreen(camera, &r))
            {
                if (!SDL_RenderTextureRotated(camera->GetWindow()->GetRenderer(), texture, NULL, &r, angle + camera->GetAngle(), NULL, SDL_FLIP_NONE))
                {
                    Console::LogError("failed to draw text: " + message);
                }
            }
            SDL_DestroySurface(textSurface);
            SDL_DestroyTexture(texture);
        }

        static void DrawLine(Camera *camera, Vector2<float> from, Vector2<float> to, ColorRGBA color = ColorRGBA(255, 255, 255, 255))
        {
            from = projectToCamera(camera, from);
            to = projectToCamera(camera, to);
            SDL_SetRenderDrawColor(camera->GetWindow()->GetRenderer(), color.red, color.green, color.blue, color.alpha);
            SDL_RenderLine(camera->GetWindow()->GetRenderer(), from.x, from.y, to.x, to.y);
        }

        static void DrawTri(Camera *camera, Vector2<float> corner1, Vector2<float> corner2, Vector2<float> corner3, ColorRGBA color = ColorRGBA(255, 255, 255, 255))
        {
            corner1 = projectToCamera(camera, corner1);
            corner2 = projectToCamera(camera, corner2);
            corner3 = projectToCamera(camera, corner3);

            SDL_Vertex vertices[3];
            // SDL_zeroa(vertices);
            vertices[0].position.x = corner1.x;
            vertices[0].position.y = corner1.y;
            vertices[0].color.r = color.red / 255.0f;
            vertices[0].color.g = color.green / 255.0f;
            vertices[0].color.b = color.blue / 255.0f;
            vertices[0].color.a = color.alpha / 255.0f;
            vertices[1].position.x = corner2.x;
            vertices[1].position.y = corner2.y;
            vertices[1].color.r = color.red / 255.0f;
            vertices[1].color.g = color.green / 255.0f;
            vertices[1].color.b = color.blue / 255.0f;
            vertices[1].color.a = color.alpha / 255.0f;
            vertices[2].position.x = corner3.x;
            vertices[2].position.y = corner3.y;
            vertices[2].color.r = color.red / 255.0f;
            vertices[2].color.g = color.green / 255.0f;
            vertices[2].color.b = color.blue / 255.0f;
            vertices[2].color.a = color.alpha / 255.0f;

            SDL_RenderGeometry(camera->GetWindow()->GetRenderer(), NULL, vertices, 3, NULL, 0);
        }
        static void DrawPixel(Camera *camera, Vector2<int> pos, ColorRGBA color = ColorRGBA(255, 255, 255, 255))
        {
            Vector2<int> drawPos = projectToCamera(camera, pos);
            SDL_SetRenderDrawColor(camera->GetWindow()->GetRenderer(), color.red, color.green, color.blue, color.alpha);
            SDL_RenderPoint(camera->GetWindow()->GetRenderer(), drawPos.x, drawPos.y);
        }
    };

};

#endif