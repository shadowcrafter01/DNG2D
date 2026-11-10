#ifndef DNG_DINGUS_HPP
#define DNG_DINGUS_HPP

#include "DNG2D/Window.hpp"
#include "DNG2D/Texture.hpp"
#include "DNG2D/Camera.hpp"
#include "DNG2D/DrawTools.hpp"
#include "DNG2D/Timer.hpp"
#include "DNG2D/Vector.hpp"
#include <vector>
#include "DNG2D/CollisionShape.hpp"
#include <functional>
#include "DNG2D/Stopwatch.hpp"
#include "DNG2D/RunOnce.hpp"
#include "DNG2D/Transform.hpp"
#include <box2d/box2d.h>
#include "DNG2D/Math.hpp"
#include "DNG2D/Action.hpp"
#include "DNG2D/AnimatedTexture.hpp"
#include "DNG2D/Timer.hpp"

#include <memory>
#include <utility>
#include <algorithm>

namespace DNG
{

    class Dingus
    {
    private:
        inline static std::vector<Dingus *> _instances;

        void _Update()
        {
            _HandleInputEvents();

            if (_physicsActive)
            {
                b2WorldTransform t = b2Body_GetTransform(_bodyID);
                _transform.position.x = t.p.x;
                _transform.position.y = t.p.y;
                _transform.angle = b2Rot_GetAngle(t.q) * -180.0 / M_PI;
            }
            if ((_texture != nullptr || _animatedTexture != nullptr) && _camera != nullptr)
            {
                if (flag_animated)
                {
                    _frame += _frameRate * _timer.GetDelta();
                    if (_frame > _animatedTexture->GetFrameCount())
                    {
                        _frame -= _animatedTexture->GetFrameCount();
                    }
                    DrawTools::DrawAnimatedTexture(_camera, _animatedTexture, _transform.position, (int)_frame, _transform.size, _transform.angle, _color);
                }
                else
                {
                    DrawTools::DrawTexture(_camera, _texture, _transform.position, _transform.size, _transform.angle, _color);
                }
            }

            Update();
        }

        void _HandleInputEvents()
        {
            if (_transform.ContainsPoint(Input::Mouse::GetMouseWorldPos(_camera)))
            {
                MouseHover();

                if (DNG::Input::Mouse::GetButtonSet().size() > 0)
                {
                    MouseDown();
                }

                _state = false;
                for (auto in : _inputs)
                {
                    if (in.isMouseInput)
                    {
                        if (Input::Mouse::GetButtonSet().contains(in.button))
                        {
                            _state = true;
                            break;
                        }
                    }
                    else
                    {
                        if (Input::Keyboard::GetKeySet().contains(in.key))
                        {
                            _state = true;
                            break;
                        }
                    }
                }
                if (_state)
                {
                    if (_flag_onPress)
                    {
                        _flag_onPress = false;
                        for (auto f : _functions_true)
                        {
                            f(this);
                        }
                    }
                    for (auto f : _functions_while)
                    {
                        f(this);
                    }
                }
                else
                {
                    _flag_onPress = true;
                }
                for (auto f : _functions_passKeyState)
                {
                    f(this, _state);
                }
                if (!_state)
                {
                    if (_flag_onRelease)
                    {
                        _flag_onRelease = false;
                        for (auto f : _functions_false)
                        {
                            f(this);
                        }
                    }
                    for (auto f : _functions_not)
                    {
                        f(this);
                    }
                }
                else
                {
                    _flag_onRelease = true;
                }
            }
        }

        inline static size_t _zeroIndex = 0;
        static void _UpdateZeroIndex()
        {
            auto i = std::lower_bound(
                _instances.begin(),
                _instances.end(),
                0.0f,
                [](const Dingus *object, float layer)
                {
                    return object->_renderLayer < layer;
                });
            _zeroIndex = static_cast<size_t>(std::distance(_instances.begin(), i));
        }

        static void _ResortInstances()
        {
            std::stable_sort(_instances.begin(),
                             _instances.end(),
                             [](const Dingus *a, const Dingus *b)
                             {
                                 return a->_renderLayer < b->_renderLayer;
                             });
            _UpdateZeroIndex();
        }

        struct InputType
        {
            InputType(SDL_Scancode keyboardKey)
            {
                isMouseInput = false;
                key = keyboardKey;
            }
            InputType(Uint8 mouseButton)
            {
                isMouseInput = true;
                button = mouseButton;
            }
            bool isMouseInput;
            SDL_Scancode key;
            Uint8 button;
        };
        std::vector<InputType> _inputs;

        b2BodyId _bodyID;
        static inline b2WorldDef _worldDef = b2DefaultWorldDef();
        static inline b2WorldId _worldID = b2CreateWorld(&_worldDef);
        static inline float _worldTstep = 1.0f / 60.0f;
        static inline int _worldSubsteps = 8;
        Transform2<float> _transform = Transform2<float>(0, 0, 1, 1, 0);
        CollisionShape collisionShape;
        Camera *_camera = nullptr;
        Texture *_texture = nullptr;
        AnimatedTexture *_animatedTexture = nullptr;
        bool flag_animated = false;
        float _frame = 0.0f;
        float _frameRate = 0.0f;
        inline static Timer _timer;
        ColorRGBA _color = RGBA_TEXTURE_UNCHANGED;
        float _renderLayer = 0.0f;
        const char *_name = "dingus";

        std::vector<std::function<void(Dingus *, bool)>> _functions_passKeyState;
        std::vector<std::function<void(Dingus *)>> _functions_while;
        std::vector<std::function<void(Dingus *)>> _functions_not;
        std::vector<std::function<void(Dingus *)>> _functions_true;
        std::vector<std::function<void(Dingus *)>> _functions_false;
        bool _flag_onPress = true;
        bool _flag_onRelease = true;
        bool _state = false;

        bool _active = true;
        bool _physicsActive = false;
        bool _queueForDelete = false;

        Dingus *_AddSelfToInstances()
        {
            //_instances.push_back(this);
            _instances.insert(_instances.begin() + _zeroIndex, this);
            _zeroIndex++;
            return this;
        }

    public:
        Dingus(const char *name = "dingus")
        {
            _AddSelfToInstances()->SetName(name);
        }
        Dingus(Transform2<float> transform, const char *name = "dingus")
        {
            _AddSelfToInstances()->SetTransform(transform)->SetName(name);
        }
        Dingus(Texture *texture, const char *name = "dingus")
        {
            _AddSelfToInstances()->AssignTexture(texture)->SetName(name);
        }
        Dingus(AnimatedTexture *texture, const char *name = "dingus")
        {
            _AddSelfToInstances()->AssignTexture(texture)->SetName(name);
        }
        Dingus(Camera *camera, const char *name = "dingus")
        {
            _AddSelfToInstances()->AssignCamera(camera)->SetName(name);
        }
        Dingus(Camera *camera, Texture *texture, const char *name = "dingus")
        {
            _AddSelfToInstances()->AssignCamera(camera)->AssignTexture(texture)->SetTransform(Transform2<float>(0, 0, texture->GetSize().x, texture->GetSize().y, 0))->SetName(name);
        }
        Dingus(Camera *camera, Texture *texture, Transform2<float> transform, const char *name = "dingus")
        {
            _AddSelfToInstances()->AssignCamera(camera)->AssignTexture(texture)->SetTransform(transform)->SetName(name);
        }
        Dingus(Camera *camera, AnimatedTexture *texture, const char *name = "dingus")
        {
            _AddSelfToInstances()->AssignCamera(camera)->AssignTexture(texture)->SetTransform(Transform2<float>(0, 0, texture->GetSize().x, texture->GetSize().y, 0))->SetName(name);
        }
        Dingus(Camera *camera, AnimatedTexture *texture, Transform2<float> transform, const char *name = "dingus")
        {
            _AddSelfToInstances()->AssignCamera(camera)->AssignTexture(texture)->SetTransform(transform)->SetName(name);
        }
        ~Dingus()
        {
            if (_physicsActive)
            {
                b2DestroyBody(_bodyID);
                _bodyID = b2_nullBodyId;
                _physicsActive = false;
            }
        }
        Dingus(const Dingus &) = delete;
        Dingus &operator=(const Dingus &) = delete;
        static void Destroy(Dingus *dngs)
        {
            for (const auto d : _instances)
            {
                if (d == dngs)
                {
                    d->_queueForDelete = true;
                }
            }
        }
        void Destroy()
        {
            if (_physicsActive)
            {
                b2DestroyBody(_bodyID);
                _bodyID = b2_nullBodyId;
                _physicsActive = false;
            }
            _queueForDelete = true;
        }
        static void DestroyAll()
        {
            for (auto d : _instances)
            {
                d->Destroy();
            }
            _instances.clear();
        }

        // assign functions

        /// @brief Defines collision shape and enables collision for this dingus
        /// @param collider Collision shape
        /// @param density Physics weight density
        /// @param friction Sliding friction coefficient
        /// @param restitution "Bounciness"
        Dingus *AssignCollisionShape(CollisionShape collider, float density = 1.0f, float friction = 0.25f, float restitution = 0.1f)
        {
            if (this == nullptr)
            {
                return nullptr;
            }
            if (_physicsActive)
            {
                return this;
            }

            collisionShape = collider;

            b2BodyDef bodyDef = b2DefaultBodyDef();
            bodyDef.type = b2_dynamicBody;
            bodyDef.position = _transform.position;
            bodyDef.userData = this;
            _bodyID = b2CreateBody(_worldID, &bodyDef);
            b2ShapeDef shapeDef = b2DefaultShapeDef();
            if (collider.flag_layerByGroupIndex)
            {
                shapeDef.filter.groupIndex = collider.group;
            }
            else
            {
                shapeDef.filter.categoryBits = collider.category;
                shapeDef.filter.maskBits = collider.mask;
            }
            shapeDef.density = density;
            shapeDef.material.friction = friction;
            shapeDef.material.restitution = restitution;
            shapeDef.enableContactEvents = true;

            switch (collisionShape.type)
            {
            case CollisionShape::Type::Box:
            {
                b2Polygon polygon = b2MakeBox(collisionShape.halfSize.x, collisionShape.halfSize.y);
                b2CreatePolygonShape(_bodyID, &shapeDef, &polygon);
                break;
            }
            case CollisionShape::Type::Circle:
            {
                b2Circle circle = {{0.0f, 0.0f}, collisionShape.radius};
                b2CreateCircleShape(_bodyID, &shapeDef, &circle);
                break;
            }
            case CollisionShape::Type::Capsule:
            {
                b2Capsule capsule = {collisionShape.pointA, collisionShape.pointB, collisionShape.radius};
                b2CreateCapsuleShape(_bodyID, &shapeDef, &capsule);
                break;
            }
            case CollisionShape::Type::Polygon:
            {
                if (collisionShape.vertices.size() < 3)
                {
                    b2DestroyBody(_bodyID);
                    _bodyID = b2_nullBodyId;
                    throw std::invalid_argument("Polygon needs at least 3 vertices");
                }
                b2Hull hull = b2ComputeHull(collisionShape.vertices.data(), static_cast<int>(collisionShape.vertices.size()));
                if (hull.count < 3)
                {
                    b2DestroyBody(_bodyID);
                    _bodyID = b2_nullBodyId;
                    throw std::invalid_argument("Invalid polygon geometry");
                }
                b2Polygon polygon = b2MakePolygon(&hull, 0.0f);
                b2CreatePolygonShape(_bodyID, &shapeDef, &polygon);
                break;
            }
            case CollisionShape::Type::Segment:
            {
                b2Segment segment = {collisionShape.segmentA, collisionShape.segmentB};
                b2CreateSegmentShape(_bodyID, &shapeDef, &segment);
                break;
            }
            }
            _physicsActive = true;

            return this;
        }
        /// @brief Gives a rendered texture to this dingus, without a camera this will render in screen space (top left origin, position is window size)
        /// @param tex Pointer reference to a texture object
        Dingus *AssignTexture(Texture *tex)
        {
            if (this == nullptr)
            {
                return nullptr;
            }
            _texture = tex;
            flag_animated = false;
            return this;
        }
        Dingus *AssignTexture(AnimatedTexture *tex)
        {
            if (this == nullptr)
            {
                return nullptr;
            }
            _animatedTexture = tex;
            flag_animated = true;
            return this;
        }
        /// @brief Assigns a camera to this dingus's position transforms, position will be defined relative to this camera with or without a texture
        /// @param cam Camera object reference
        Dingus *AssignCamera(Camera *cam)
        {
            if (this == nullptr)
            {
                return nullptr;
            }
            _camera = cam;
            return this;
        }
        Dingus *AssignAction(std::function<void(Dingus *, bool)> f, SDL_Scancode key)
        {
            if (this == nullptr)
            {
                return nullptr;
            }
            AssignKey(key);
            AssignFunction(f);
            return this;
        }
        Dingus *AssignAction(std::function<void(Dingus *, bool)> f, Uint8 button)
        {
            if (this == nullptr)
            {
                return nullptr;
            }
            AssignKey(button);
            AssignFunction(f);
            return this;
        }
        Dingus *AssignAction(std::function<void(Dingus *)> f, SDL_Scancode key, int flag = ACTION_WHILE_HELD)
        {
            if (this == nullptr)
            {
                return nullptr;
            }
            AssignKey(key);
            AssignFunction(f, flag);
            return this;
        }
        Dingus *AssignAction(std::function<void(Dingus *)> f, Uint8 button, int flag = ACTION_WHILE_HELD)
        {
            if (this == nullptr)
            {
                return nullptr;
            }
            AssignKey(button);
            AssignFunction(f, flag);
            return this;
        }
        Dingus *AssignFunction(std::function<void(Dingus *)> f, int flag = ACTION_WHILE_HELD)
        {
            if (this == nullptr)
            {
                return nullptr;
            }
            switch (flag)
            {
            case ACTION_WHILE_HELD:
                _functions_while.push_back(f);
                break;
            case ACTION_ON_PRESS:
                _functions_true.push_back(f);
                break;
            case ACTION_ON_RELEASE:
                _functions_false.push_back(f);
                break;
            case ACTION_NOT_HELD:
                _functions_not.push_back(f);
                break;

            default:
                break;
            }
            return this;
        }
        Dingus *AssignFunction(std::function<void(Dingus *, bool)> f)
        {
            if (this == nullptr)
            {
                return nullptr;
            }
            _functions_passKeyState.push_back(f);
            return this;
        }
        Dingus *AssignKey(SDL_Scancode key)
        {
            if (this == nullptr)
            {
                return nullptr;
            }
            InputType newInput(key);
            _inputs.push_back(newInput);
            return this;
        }
        Dingus *AssignKey(Uint8 button)
        {
            if (this == nullptr)
            {
                return nullptr;
            }
            InputType newInput(button);
            _inputs.push_back(newInput);
            return this;
        }

        // get functions
        Transform2<float> GetTransform()
        {
            if (this == nullptr)
            {
                return Transform2<float>();
            }
            return _transform;
        }
        Vector2<float> GetPosition()
        {
            if (this == nullptr)
            {
                return Vector2<float>();
            }
            return _transform.position;
        }
        b2BodyId GetBodyID()
        {
            if (this == nullptr)
            {
                return b2_nullBodyId;
            }
            if (_physicsActive)
            {
                return _bodyID;
            }
            else
            {
                return b2_nullBodyId;
            }
        }
        Vector2<float> GetVelocity()
        {
            if (this == nullptr)
            {
                return Vector2<float>();
            }
            return Vector2<float>::B2D_to_DNG(b2Body_GetLinearVelocity(_bodyID));
        }
        Texture *GetTexture()
        {
            if (this == nullptr)
            {
                return nullptr;
            }
            return _texture;
        }
        Camera *GetCamera()
        {
            if (this == nullptr)
            {
                return nullptr;
            }
            return _camera;
        }
        float GetRenderLayer()
        {
            if (this == nullptr)
            {
                return 0;
            }
            return _renderLayer;
        }
        const char *GetName()
        {
            if (this == nullptr)
            {
                return "null";
            }
            return _name;
        }
        static Dingus *GetNearest(Vector2<float> position)
        {
            Dingus *nearestDingus = nullptr;
            float nearestDistance = std::numeric_limits<float>::max();

            for (const auto dngs : _instances)
            {
                float dist = Vector2<float>::Distance(position, dngs->GetPosition());
                if (dist < nearestDistance)
                {
                    nearestDistance = dist;
                    nearestDingus = dngs;
                }
            }
            return nearestDingus;
        }
        static Dingus *GetNearest(Vector2<float> position, const char *name)
        {
            Dingus *nearestDingus = nullptr;
            float nearestDistance = std::numeric_limits<float>::max();

            for (const auto dngs : _instances)
            {
                float dist = Vector2<float>::Distance(position, dngs->GetPosition());
                if ((dngs->GetName() == name) && (dist < nearestDistance))
                {
                    nearestDistance = dist;
                    nearestDingus = dngs;
                }
            }
            return nearestDingus;
        }
        static Dingus *GetByName(const char *name)
        {
            for (const auto dngs : _instances)
            {
                if (dngs->GetName() == name)
                {
                    return dngs;
                }
            }
            return nullptr;
        }
        static Dingus *GetByIndex(int index)
        {
            return _instances.at(index);
        }
        static std::vector<Dingus *> GetAllByName(const char *name)
        {
            std::vector<Dingus *> array = {};
            for (const auto dngs : _instances)
            {
                if (dngs->GetName() == name)
                {
                    array.push_back(dngs);
                }
            }
            return array;
        }
        static const std::vector<Dingus *> &GetAllInstances()
        {
            return _instances;
        }
        static std::vector<Dingus *> GetAllInRange(Vector2<float> position, float radius)
        {
            std::vector<Dingus *> dinguses = {};

            for (const auto dngs : _instances)
            {
                float dist = Vector2<float>::Distance(position, dngs->GetPosition());
                if (dist <= radius)
                {
                    dinguses.push_back(dngs);
                }
            }
            return dinguses;
        }
        static std::vector<Dingus *> GetAllInRange(Vector2<float> position, float radius, const char *name)
        {
            std::vector<Dingus *> dinguses = {};

            for (const auto dngs : _instances)
            {
                if (dngs->GetName() == name)
                {
                    float dist = Vector2<float>::Distance(position, dngs->GetPosition());
                    if (dist <= radius)
                    {
                        dinguses.push_back(dngs);
                    }
                }
            }
            return dinguses;
        }
        static const b2WorldId GetWorld()
        {
            return _worldID;
        }

        // set functions
        Dingus *SetTransform(Transform2<float> t)
        {
            if (this == nullptr)
            {
                return nullptr;
            }
            if (_physicsActive)
            {
                b2Body_SetTransform(_bodyID, t, t);
                _transform.size = t.size;
                b2Body_SetAwake(_bodyID, true);
            }
            else
            {
                _transform = t;
            }
            return this;
        }
        Dingus *SetPosition(Vector2<float> p)
        {
            if (this == nullptr)
            {
                return nullptr;
            }
            if (_physicsActive)
            {
                b2Body_SetTransform(_bodyID, p, _transform);
                b2Body_SetAwake(_bodyID, true);
            }
            else
            {
                _transform.position = p;
            }
            return this;
        }
        Dingus *SetPosition(float x, float y)
        {
            if (this == nullptr)
            {
                return nullptr;
            }
            if (_physicsActive)
            {
                b2Body_SetTransform(_bodyID, (b2Vec2){x, y}, _transform);
            }
            else
            {
                _transform.position.x = x;
                _transform.position.y = y;
            }
            return this;
        }
        Dingus *SetAngle(float deg)
        {
            if (this == nullptr)
            {
                return nullptr;
            }
            if (_physicsActive)
            {
                b2Body_SetTransform(_bodyID, _transform, b2MakeRot(Math::deg2rad(deg)));
            }
            else
            {
                _transform.angle = deg;
            }
            return this;
        }
        Dingus *SetStatic(bool isStatic = true)
        {
            if (this == nullptr)
            {
                return nullptr;
            }
            if (_physicsActive)
            {
                b2Body_SetType(_bodyID, (isStatic) ? b2_staticBody : b2_dynamicBody);
            }
            return this;
        }
        Dingus *SetVelocity(float u, float v)
        {
            if (this == nullptr)
            {
                return nullptr;
            }
            b2Body_SetLinearVelocity(_bodyID, {u, v});
            return this;
        }
        Dingus *SetVelocity(Vector2<float> v)
        {
            if (this == nullptr)
            {
                return nullptr;
            }
            b2Body_SetLinearVelocity(_bodyID, v);
            return this;
        }
        Dingus *SetRenderLayer(float layer)
        {
            if (this == nullptr)
            {
                return nullptr;
            }
            if (_renderLayer == layer)
            {
                return this;
            }
            _renderLayer = layer;
            _ResortInstances();
            return this;
        }
        Dingus *SetName(const char *name)
        {
            if (this == nullptr)
            {
                return nullptr;
            }
            _name = name;
            return this;
        }
        Dingus *SetColorRGBA(ColorRGBA color)
        {
            if (this == nullptr)
            {
                return nullptr;
            }
            _color = color;
            return this;
        }
        Dingus *SetColorRGBA(Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha)
        {
            if (this == nullptr)
            {
                return nullptr;
            }
            ColorRGBA color(red, green, blue, alpha);
            _color = color;
            return this;
        }
        Dingus *SetFramerate(float framerate = 0.0f)
        {
            if (this == nullptr)
            {
                return nullptr;
            }
            _frameRate = framerate;
            return this;
        }
        Dingus *SetFrame(int frame = 0)
        {
            if (this == nullptr)
            {
                return nullptr;
            }
            _frame = frame;
            return this;
        }
        
        static void SetWorldGravity(Vector2<float> v)
        {
            b2World_SetGravity(_worldID, v);
        }
        static void SetWorldTimeStep(float t)
        {
            _worldTstep = t;
        }

        // apply functions
        Dingus *ApplyForce(Vector2<float> f)
        {
            if (this == nullptr)
            {
                return nullptr;
            }
            if (_physicsActive)
            {
                b2Body_ApplyForceToCenter(_bodyID, f, true);
            }
            else
            {
            }
            return this;
        }
        Dingus *ApplyForce(float Fx, float Fy)
        {
            if (this == nullptr)
            {
                return nullptr;
            }
            if (_physicsActive)
            {
                b2Body_ApplyForceToCenter(_bodyID, (b2Vec2){Fx, Fy}, true);
            }
            else
            {
            }
            return this;
        }
        Dingus *ApplyForceAt(Vector2<float> f, Vector2<float> p)
        {
            if (this == nullptr)
            {
                return nullptr;
            }
            if (_physicsActive)
            {
                b2Body_ApplyForce(_bodyID, f, p, true);
            }
            else
            {
            }
            return this;
        }
        Dingus *ApplyImpulse(Vector2<float> i)
        {
            if (this == nullptr)
            {
                return nullptr;
            }
            if (_physicsActive)
            {
                b2Body_ApplyLinearImpulseToCenter(_bodyID, i, true);
            }
            else
            {
            }
            return this;
        }
        Dingus *ApplyImpulse(float Ix, float Iy)
        {
            if (this == nullptr)
            {
                return nullptr;
            }
            if (_physicsActive)
            {
                b2Body_ApplyLinearImpulseToCenter(_bodyID, (b2Vec2){Ix, Iy}, true);
            }
            else
            {
            }
            return this;
        }
        Dingus *ApplyImpulseAt(Vector2<float> i, Vector2<float> p)
        {
            if (this == nullptr)
            {
                return nullptr;
            }
            if (_physicsActive)
            {
                b2Body_ApplyLinearImpulse(_bodyID, i, p, true);
            }
            else
            {
            }
            return this;
        }
        Dingus *ApplyTorque(float t)
        {
            if (this == nullptr)
            {
                return nullptr;
            }
            b2Body_ApplyTorque(_bodyID, t, true);
            return this;
        }

        /// virtual update function that you override (if you want) in any class that uses Dingus as a parent class
        /// is called when drawn to screen
        virtual void Update()
        {
        }
        virtual void MouseDown(Uint8 button)
        {
        }
        virtual void MouseDown()
        {
        }
        virtual void MouseHover()
        {
        }
        virtual void OnCollision(Dingus *other)
        {
        }
        virtual void OnCollision()
        {
        }

        // update all function called by the engine, no touchie
        inline static void UpdateAll()
        {
            _timer.Update();

            // update box2d for all
            b2World_Step(_worldID, _worldTstep, _worldSubsteps);

            // get all collision data wooooo
            b2ContactEvents collisions = b2World_GetContactEvents(_worldID);
            for (int i = 0; i < collisions.beginCount; i++)
            {
                b2ContactBeginTouchEvent *collision = &collisions.beginEvents[i];

                b2BodyId box1 = b2Shape_GetBody(collision->shapeIdA);
                b2BodyId box2 = b2Shape_GetBody(collision->shapeIdB);

                Dingus *dingus1 = static_cast<Dingus *>(b2Body_GetUserData(box1));
                Dingus *dingus2 = static_cast<Dingus *>(b2Body_GetUserData(box2));

                if (dingus1 != nullptr && dingus2 != nullptr)
                {
                    dingus1->OnCollision(dingus2);
                    dingus2->OnCollision(dingus1);
                    dingus1->OnCollision();
                    dingus2->OnCollision();
                }
            }

            // create a new list of pointers so if more are added or deleted during the input actions then the actual list of ones being updated doesnt change size
            std::vector<Dingus *> updateList = _instances;

            int i = 0;
            for (auto d : updateList)
            {
                if (d->_queueForDelete)
                {
                    delete d;                                 // delete the actual dingus the pointer links to
                    _instances.erase(_instances.begin() + i); // remove the pointer from the instances list
                    _UpdateZeroIndex();
                }
                else
                {
                    i++;
                    d->_Update();
                }
            }
        }
    };
};

#endif