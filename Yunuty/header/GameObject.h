#pragma once
#include <iostream>
#include "Object.h"
#include "IGameObjectParent.h"
#include <unordered_map>
#include "Transform.h"
#include <functional>

#ifdef YUNUTY_EXPORTS
#define YUNUTY_API __declspec(dllexport)
#else
#define YUNUTY_API __declspec(dllimport)
#endif

using namespace std;
namespace YunutyEngine
{
    class Scene;
    class Component;
    class Transform;
    class YunutyCycle;
    class YUNUTY_API GameObject :
        public Object, public IGameObjectParent
    {
    private:
        Scene* scene = nullptr;
        string name="";
        bool selfActive = true;
        int childrenNum=0;
        IGameObjectParent* parent = nullptr;
        GameObject* parentGameObject = nullptr;
        Transform* transform;
        unordered_map<GameObject*, unique_ptr<GameObject>> children;
        vector<GameObject*> childrenIndexed;
        unordered_map<GameObject*, int> childIndexMap;
        unordered_map<Component*, unique_ptr<Component>> components;
        GameObject(IGameObjectParent* parent);
        void DoThingsOnParents(function<void(GameObject*)> todo);
    protected:
        unique_ptr<YunutyEngine::GameObject> MoveChild(GameObject* child) override;
        void ReceiveChild(remove_reference<unique_ptr<GameObject>>::type&& child) override;
        int GetChildIndex(GameObject* child) override;
    public:
        GameObject(GameObject&) = delete;
        GameObject& operator=(GameObject&) = delete;
        GameObject() = default;
        virtual const vector<GameObject*>& GetChildren()const override;
        ~GameObject();
        Transform* GetTransform();
        template<typename ComponentType>
        ComponentType* AddComponent()
        {
            static_assert(std::is_base_of<Component, ComponentType>::value, "only derived classes from component are allowed");
            auto newComponent = new ComponentType();
            newComponent->gameObject = this;
            components.insert(make_pair(newComponent, unique_ptr<Component>(newComponent)));
            return newComponent;
        }
        template<typename ComponentType>
        ComponentType* GetComponent()
        {
            static_assert(std::is_base_of<Component, ComponentType>::value, "only derived classes from component are allowed");
            for (auto i = components.begin(); i != components.end(); i++)
            {
                auto castedPointer = dynamic_cast<ComponentType*>(i->first);
                if (castedPointer)
                    return castedPointer;;
            }
            return nullptr;
        }
        template<typename ComponentType>
        vector<ComponentType*> GetComponents()
        {
            vector<ComponentType*> ret;
            static_assert(std::is_base_of<Component, ComponentType>::value, "only derived classes from component are allowed");
            for (auto i = components.begin(); i != components.end(); i++)
            {
                auto castedPointer = dynamic_cast<ComponentType*>(i->first);
                if (castedPointer)
                    ret.push_back(castedPointer);
            }
            return ret;
        }
        vector<Component*> GetComponents() { return GetComponents<Component>(); }
        void DeleteComponent(Component* component);
        bool GetActive();
        bool GetSelfActive();
        void SetSelfActive(bool selfActive);
        GameObject* GetParentGameObject();
        Scene* GetScene();
        void SetParent(IGameObjectParent* parent = nullptr);
        int GetChildIndex();
        int GetSceneIndex();
        string getName()const;
        void setName(const string& name);

        friend Scene;
        friend YunutyCycle;
    };
}
