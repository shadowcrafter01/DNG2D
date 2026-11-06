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
#include <box2d/base.h>
#include <box2d/box2d.h>
#include "DNG2D/Math.hpp"
#include "DNG2D/Action.hpp"
#include "DNG2D/AnimatedTexture.hpp"

#include <memory>
#include <utility>
#include <algorithm>

namespace DNG
{

    class Dingus
    {
    private:
        // inline static std::vector<std::unique_ptr<Dingus>> _instances;
        inline static std::vector<Dingus *> _instances;

        void _Update()
        {
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
                    DrawTools::DrawAnimatedTexture(_camera, _animatedTexture, _transform.position, 1, _transform.size, _transform.angle, _color);
                }
                else
                {
                    DrawTools::DrawTexture(_camera, _texture, _transform.position, _transform.size, _transform.angle, _color);
                }
            }

            bool hover = _transform.ContainsPoint(Input::Mouse::GetMouseWorldPos(_camera));
            if (hover)
            {
                _HandleInputEvents();
            }

            Update();
        }

        void _HandleInputEvents()
        {
            _state = false;
            for (auto in : _inputs)
            {
                if (in.isMouseInput)
                {
                    if (Input::Mouse::GetButtonSet().count(in.button) > 0)
                    {
                        _state = true;
                        break;
                    }
                }
                else
                {
                    if (Input::Keyboard::GetKeySet().count(in.key) > 0)
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

        inline static size_t _zeroIndex = 0;
        // static void _UpdateZeroIndex()
        //{
        //     auto i = std::lower_bound(
        //         _instances.begin(),
        //         _instances.end(),
        //         0.0f,
        //         [](const std::unique_ptr<Dingus> &object, float layer)
        //         {
        //             return object->_renderLayer < layer;
        //         });
        //     _zeroIndex = static_cast<size_t>(std::distance(_instances.begin(), i));
        // }

        // static void _ResortInstances()
        //{
        //     std::stable_sort(_instances.begin(),
        //                      _instances.end(),
        //                      [](const std::unique_ptr<Dingus> &a, const std::unique_ptr<Dingus> &b)
        //                      {
        //                          return a->_renderLayer < b->_renderLayer;
        //                      });
        // }
        static void _ResortInstances()
        {
            std::stable_sort(_instances.begin(),
                             _instances.end(),
                             [](const Dingus *a, const Dingus *b)
                             {
                                 return a->_renderLayer < b->_renderLayer;
                             });
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

        // constructors
        // Dingus()
        //{
        //}
        // Dingus(Camera *camera, Transform2<float> transform) : _camera{camera},
        //                                                      _transform{transform}
        //{
        //}
        // Dingus(Camera *camera, Vector2<float> pos = Vector2<float>(0, 0), float angle = 0, Vector2<float> size = Vector2<float>(0, 0)) : _camera{camera}
        //{
        //    _transform.position = pos;
        //    _transform.size = size;
        //    _transform.angle = angle;
        //}
        // Dingus(Camera *camera, Texture *texture, Transform2<float> _transform) : _camera{camera},
        //                                                                         _texture{texture},
        //                                                                         _transform{_transform}
        //{
        //}
        // Dingus(Camera *camera, Texture *texture, Vector2<float> pos = Vector2<float>(0, 0), float angle = 0, Vector2<float> size = Vector2<float>(0, 0)) : _camera{camera},
        //                                                                                                                                                   _texture{texture}
        //{
        //    if (size = Vector2<float>(0, 0))
        //    {
        //        _transform.size = texture->GetSize();
        //    }
        //    else
        //    {
        //        _transform.size = size;
        //    }
        //    _transform.position = pos;
        //    _transform.angle = angle;
        //}
        // Dingus(Camera *camera, AnimatedTexture *texture, Transform2<float> _transform) : _camera{camera},
        //                                                                                 _animatedTexture{texture},
        //                                                                                 _transform{_transform}
        //{
        //    flag_animated = true;
        //}
        // Dingus(Camera *camera, AnimatedTexture *texture, Vector2<float> pos = Vector2<float>(0, 0), float angle = 0, Vector2<float> size = Vector2<float>(0, 0)) : _camera{camera},
        //                                                                                                                                                           _animatedTexture{texture}
        //{
        //    if (size = Vector2<float>(0, 0))
        //    {
        //        _transform.size = texture->GetSize();
        //    }
        //    else
        //    {
        //        _transform.size = size;
        //    }
        //    _transform.position = pos;
        //    _transform.angle = angle;
        //    flag_animated = true;
        //}

        Dingus *_AddSelfToInstances()
        {
            // auto dngs = std::unique_ptr<Dingus>(this);
            // dngs->_renderLayer = 0.0f;
            // Dingus *ptr = dngs.get();
            //_instances.insert(_instances.begin() + _zeroIndex, std::move(dngs));
            //++_zeroIndex;
            // return ptr;
            _instances.push_back(this);
            return this;
        }

    public:
        Dingus()
        {
            _AddSelfToInstances();
        }
        Dingus(Transform2<float> transform)
        {
            _AddSelfToInstances()->SetTransform(transform);
        }
        Dingus(Texture *texture)
        {
            _AddSelfToInstances()->AssignTexture(texture);
        }
        Dingus(AnimatedTexture *texture)
        {
            _AddSelfToInstances()->AssignTexture(texture);
        }
        Dingus(Camera *camera)
        {
            _AddSelfToInstances()->AssignCamera(camera);
        }
        Dingus(Camera *camera, Texture *texture)
        {
            _AddSelfToInstances()->AssignCamera(camera)->AssignTexture(texture)->SetTransform(Transform2<float>(0, 0, texture->GetSize().x, texture->GetSize().y, 0));
        }
        Dingus(Camera *camera, Texture *texture, Transform2<float> transform)
        {
            _AddSelfToInstances()->AssignCamera(camera)->AssignTexture(texture)->SetTransform(transform);
        }
        // static Dingus *Create(Camera *camera, Transform2<float> transform)
        //{
        //     auto dngs = std::unique_ptr<Dingus>(new Dingus(camera, transform));
        //     dngs->_renderLayer = 0.0f;
        //     Dingus *ptr = dngs.get();
        //     _instances.insert(_instances.begin() + _zeroIndex, std::move(dngs));
        //     ++_zeroIndex;
        //     return ptr;
        // }
        // static Dingus *Create(Camera *camera, Vector2<float> pos = Vector2<float>(0, 0), float angle = 0, Vector2<float> size = Vector2<float>(0, 0))
        //{
        //     auto dngs = std::unique_ptr<Dingus>(new Dingus(camera, pos, angle, size));
        //     dngs->_renderLayer = 0.0f;
        //     Dingus *ptr = dngs.get();
        //     _instances.insert(_instances.begin() + _zeroIndex, std::move(dngs));
        //     ++_zeroIndex;
        //     return ptr;
        // }
        // static Dingus *Create(Camera *camera, Texture *texture, Transform2<float> transform)
        //{
        //     auto dngs = std::unique_ptr<Dingus>(new Dingus(camera, texture, transform));
        //     dngs->_renderLayer = 0.0f;
        //     Dingus *ptr = dngs.get();
        //     _instances.insert(_instances.begin() + _zeroIndex, std::move(dngs));
        //     ++_zeroIndex;
        //     return ptr;
        // }
        // static Dingus *Create(Camera *camera, Texture *texture, Vector2<float> pos = Vector2<float>(0, 0), float angle = 0, Vector2<float> size = Vector2<float>(0, 0))
        //{
        //     auto dngs = std::unique_ptr<Dingus>(new Dingus(camera, texture, pos, angle, size));
        //     dngs->_renderLayer = 0.0f;
        //     Dingus *ptr = dngs.get();
        //     _instances.insert(_instances.begin() + _zeroIndex, std::move(dngs));
        //     ++_zeroIndex;
        //     return ptr;
        // }
        // static Dingus *Create(Camera *camera, AnimatedTexture *texture, Transform2<float> transform)
        //{
        //     auto dngs = std::unique_ptr<Dingus>(new Dingus(camera, texture, transform));
        //     dngs->_renderLayer = 0.0f;
        //     Dingus *ptr = dngs.get();
        //     _instances.insert(_instances.begin() + _zeroIndex, std::move(dngs));
        //     ++_zeroIndex;
        //     return ptr;
        // }
        // static Dingus *Create(Camera *camera, AnimatedTexture *texture, Vector2<float> pos = Vector2<float>(0, 0), float angle = 0, Vector2<float> size = Vector2<float>(0, 0))
        //{
        //     auto dngs = std::unique_ptr<Dingus>(new Dingus(camera, texture, pos, angle, size));
        //     dngs->_renderLayer = 0.0f;
        //     Dingus *ptr = dngs.get();
        //     _instances.insert(_instances.begin() + _zeroIndex, std::move(dngs));
        //     ++_zeroIndex;
        //     return ptr;
        // }
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
            // for (auto i = _instances.begin(); i != _instances.end(); ++i)
            //{
            //     if (i->get() == dngs)
            //     {
            //         //_instances.erase(i);
            //         i->get()->_queueForDelete = true;
            //         return;
            //     }
            // }
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
            // for (auto i = _instances.begin(); i != _instances.end(); ++i)
            //{
            //     if (i->get() == this)
            //     {
            //         _instances.erase(i);
            //         return;
            //     }
            // }
            //_ResortInstances();
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

        // update all function called by the Dngine, no touchie
        inline static void UpdateAll()
        {
            b2World_Step(_worldID, _worldTstep, _worldSubsteps);

            // for (unsigned int i = _instances.size(); i-- > 0; )
            //{
            //     Dingus *dingus = _instances.at(i);
            //     if (dingus->_queueForDelete)
            //     {
            //         delete _instances[i];
            //         _instances.erase(_instances.begin() + i);
            //     }
            //     else
            //     {
            //         if (dingus->_active)
            //         {
            //             dingus->_Update();
            //         }
            //     }
            // }
            for (auto i = _instances.begin(); i != _instances.end();)
            {
                if ((*i)->_queueForDelete)
                {
                    (*i)->Destroy();
                }
                else
                {
                    (*i)->_Update();
                    ++i;
                }
            }
        }
    };
};

#endif