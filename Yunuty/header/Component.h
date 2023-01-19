#pragma once
#include "Object.h"
//#include "GameObject.h"

#ifdef YUNUTY_EXPORTS
#define YUNUTY_API __declspec(dllexport)
#else
#define YUNUTY_API __declspec(dllimport)
#endif

namespace YunutyEngine
{
    class GameObject;
    class YunutyCycle;
    class Collision;
    class Collision2D;
    class Collider2D;
    class Collider;
    class Transform;
    class YUNUTY_API Component :
        public Object
    {
    private:
        GameObject* gameObject = nullptr;
        bool StartCalled = false;
    protected:
        Component() {}
        virtual void OnEnable() {};
        virtual void OnDisable() {};
        virtual void Start() {};
        virtual void Update() {};
        virtual void FixedUpdate() {};
        virtual void OnCollisionEnter(const Collision& collision) {};
        virtual void OnCollisionEnter2D(const Collision2D& collision) {};
        virtual void OnCollisionExit(const Collision& collision) {};
        virtual void OnCollisionExit2D(const Collision2D& collision) {};
        virtual void OnCollisionStay(const Collision& collision) {};
        virtual void OnCollisionStay2D(const Collision2D& collision) {};
    public:
        virtual ~Component() {}
        Transform* GetTransform()const;
        GameObject* GetGameObject()const;
        //template<typename ComponentType>
        //friend ComponentType* GameObject::AddComponent<ComponentType>();
        friend GameObject;
        friend YunutyCycle;
        friend Collider2D;
        friend Collider;
    };
}
