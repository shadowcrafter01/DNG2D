#ifndef ENG_COLLISIONSHAPE_HPP
#define ENG_COLLISIONSHAPE_HPP

#include "ENG2D/Vector.hpp"
#include <vector>

namespace ENG
{

    class CollisionShape
    {
    private:
        

    public:

        enum class Type
        {
            Box,
            Circle,
            Capsule,
            Polygon,
            Segment
        };

        Type type = Type::Box;

        // box
        b2Vec2 halfSize = {0.5f, 0.5f};

        // circle
        float radius = 0.5f;

        // capsule
        b2Vec2 pointA = {-0.5f, 0.0f};
        b2Vec2 pointB = {0.5f, 0.0f};

        // custom collision shape
        std::vector<b2Vec2> vertices;

        // line
        b2Vec2 segmentA = {-0.5f, 0.0f};
        b2Vec2 segmentB = {0.5f, 0.0f};

        // collision layer
        uint64_t category;
        uint64_t mask;
        int group;
        bool flag_layerByGroupIndex = false;

        static CollisionShape CreateBox(float width, float height, int groupIndex = 0)
        {
            CollisionShape s;
            s.type = Type::Box;
            s.halfSize = {width * 0.5f, height * 0.5f};
            s.group = groupIndex;
            s.flag_layerByGroupIndex = true;
            return s;
        }
        static CollisionShape CreateBox(Vector2<float> size, int groupIndex = 0)
        {
            CollisionShape s;
            s.type = Type::Box;
            s.halfSize = {size.x * 0.5f, size.y * 0.5f};
            s.group = groupIndex;
            s.flag_layerByGroupIndex = true;
            return s;
        }
        static CollisionShape CreateBox(float width, float height, uint64_t categoryBits, uint64_t maskBits)
        {
            CollisionShape s;
            s.type = Type::Box;
            s.halfSize = {width * 0.5f, height * 0.5f};
            s.mask = maskBits;
            s.category = categoryBits;
            return s;
        }
        static CollisionShape CreateBox(Vector2<float> size, uint64_t categoryBits, uint64_t maskBits)
        {
            CollisionShape s;
            s.type = Type::Box;
            s.halfSize = {size.x * 0.5f, size.y * 0.5f};
            s.mask = maskBits;
            s.category = categoryBits;
            return s;
        }

        static CollisionShape CreateCircle(float radius, int groupIndex = 0)
        {
            CollisionShape s;
            s.type = Type::Circle;
            s.radius = radius;
            s.group = groupIndex;
            s.flag_layerByGroupIndex = true;
            return s;
        }
        static CollisionShape CreateCircle(float radius, uint64_t categoryBits, uint64_t maskBits)
        {
            CollisionShape s;
            s.type = Type::Circle;
            s.radius = radius;
            s.mask = maskBits;
            s.category = categoryBits;
            return s;
        }

        static CollisionShape CreateCapsule(b2Vec2 a, b2Vec2 b, float radius, int groupIndex = 0)
        {
            CollisionShape s;
            s.type = Type::Capsule;
            s.pointA = a;
            s.pointB = b;
            s.radius = radius;
            s.group = groupIndex;
            s.flag_layerByGroupIndex = true;
            return s;
        }
        static CollisionShape CreateCapsule(b2Vec2 a, b2Vec2 b, float radius, uint64_t categoryBits, uint64_t maskBits)
        {
            CollisionShape s;
            s.type = Type::Capsule;
            s.pointA = a;
            s.pointB = b;
            s.radius = radius;
            s.mask = maskBits;
            s.category = categoryBits;
            return s;
        }

        static CollisionShape CreatePolygon(std::vector<b2Vec2> points, int groupIndex = 0)
        {
            CollisionShape s;
            s.type = Type::Polygon;
            s.vertices = std::move(points);
            s.group = groupIndex;
            s.flag_layerByGroupIndex = true;
            return s;
        }
        static CollisionShape CreatePolygon(std::vector<b2Vec2> points, uint64_t categoryBits, uint64_t maskBits)
        {
            CollisionShape s;
            s.type = Type::Polygon;
            s.vertices = std::move(points);
            s.mask = maskBits;
            s.category = categoryBits;
            return s;
        }

        static CollisionShape CreateSegment(b2Vec2 a, b2Vec2 b, int groupIndex = 0)
        {
            CollisionShape s;
            s.type = Type::Segment;
            s.segmentA = a;
            s.segmentB = b;
            s.group = groupIndex;
            s.flag_layerByGroupIndex = true;
            return s;
        }
        static CollisionShape CreateSegment(b2Vec2 a, b2Vec2 b, uint64_t categoryBits, uint64_t maskBits)
        {
            CollisionShape s;
            s.type = Type::Segment;
            s.segmentA = a;
            s.segmentB = b;
            s.mask = maskBits;
            s.category = categoryBits;
            return s;
        }
    };
};
#endif