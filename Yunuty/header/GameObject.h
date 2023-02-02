#pragma once
#include <iostream>
#include "Object.h"
#include "IGameObjectParent.h"
#include <unordered_map>
#include "Transform.h"
#include "Cache.h"
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
    /// <summary>
    /// ���� ������Ʈ�� ���� ���� ��ġ�� �� �ִ� ���� �⺻���� ������ ��ü�Դϴ�.
    /// ���� ������Ʈ�� �⺻������ Transform ������Ʈ�� ���� ������, �پ��� ������Ʈ�� �߰��� �������μ� ���ӿ�����Ʈ�μ��� ������ �����˴ϴ�.
    /// ���� ������Ʈ�� �ڽ� ���� ������Ʈ�� �θ� ���� ������Ʈ�� ���� �� �ִ� ���� ������ ���� �ֽ��ϴ�.
    /// �ڽ� ���� ������Ʈ�� ��ġ�� ������ �θ� ���� ������Ʈ�� ��ġ�� ������ �������Դϴ�.
    /// </summary>
    class YUNUTY_API GameObject :
        public Object, public IGameObjectParent
    {
    public:
        GameObject(GameObject&) = delete;
        GameObject& operator=(GameObject&) = delete;
        GameObject() = default;
        virtual const vector<GameObject*>& GetChildren()const override;
        ~GameObject();
        Transform* GetTransform();
        const Transform* GetTransform()const;
        // ���ø� �Ű������� �־��� Ÿ���� ������Ʈ�� ���� ������Ʈ�� �����մϴ�.
        template<typename ComponentType>
        ComponentType* AddComponent()
        {
            static_assert(std::is_base_of<Component, ComponentType>::value, "only derived classes from component are allowed");
            auto newComponent = new ComponentType();
            newComponent->gameObject = this;
            components.insert(make_pair(newComponent, unique_ptr<Component>(newComponent)));
            return newComponent;
        }
        // ���ø� �Ű������� �־��� Ÿ���� ������Ʈ�� ���� ������Ʈ�� �����ϴ��� Ȯ���ϰ�, ���ʷ� Ȯ�ε� ������Ʈ ��ü�� �����͸� ��ȯ�մϴ�.
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
        // ���ø� �Ű������� �־��� Ÿ���� ������Ʈ�� ���� ������Ʈ�� �����ϴ��� Ȯ���ϰ�, ���ʷ� Ȯ�ε� ������Ʈ ��ü�� �����͸� ��ȯ�մϴ�.
        template<typename ComponentType>
        const ComponentType* GetComponent()const
        {
            static_assert(std::is_base_of<Component, ComponentType>::value, "only derived classes from component are allowed");
            for (auto i = components.begin(); i != components.end(); i++)
            {
                auto castedPointer = dynamic_cast<const ComponentType*>(i->first);
                if (castedPointer)
                    return castedPointer;;
            }
            return nullptr;
        }
        // ���ø� �Ű������� �־��� Ÿ���� ������Ʈ���� ���� ������Ʈ�� �����ϴ��� Ȯ���ϰ�, Ȯ�ε� ������Ʈ���� �����͸� ���� �����̳ʿ� ��� ��ȯ�մϴ�.
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
        // ���� ������Ʈ�� Ȱ��ȭ�Ǿ� �ִ����� ���θ� ��ȯ�մϴ�.
        // �θ� ��ü�� Ȱ��ȭ�Ǿ� ���� �ʴٸ� ������ ��Ȱ��ȭ�� ���·� ǥ�õ˴ϴ�.
        bool GetActive();
        // ���� ������Ʈ�� ������ Ȱ��ȭ�Ǿ� �ִ����� ���θ� ��ȯ�մϴ�.
        // �θ� ��ü�� Ȱ��ȭ�Ǿ� ���� �ʴٸ� ���������� Ȱ��ȭ�� ���´� �ƴҼ��� �ֽ��ϴ�.
        bool GetSelfActive();
        void SetSelfActive(bool selfActive);
        // ���� ������Ʈ�� �θ� ���� ������Ʈ�� ��ȯ�մϴ�. �θ� ���ٸ� null �����͸� ��ȯ�մϴ�.
        GameObject* GetParentGameObject();
        const GameObject* GetParentGameObject()const;
        Scene* GetScene();
        void SetParent(IGameObjectParent* parent = nullptr);
        int GetChildIndex()const;
        int GetSceneIndex()const;
        string getName()const;
        void setName(const string& name);
        void SetCacheDirty();
    private:
        Scene* scene = nullptr;
        string name = "";
        bool selfActive = true;
        int childrenNum = 0;
        IGameObjectParent* parent = nullptr;
        GameObject* parentGameObject = nullptr;
        Transform* transform=nullptr;
        unordered_map<GameObject*, unique_ptr<GameObject>> children;
        vector<GameObject*> childrenIndexed;
        unordered_map<const GameObject*, int> childIndexMap;
        // sceneIndex is Updated per every cycle.
        int sceneIndex=0;
        mutable cache<int> cachedSceneIndex;
        unordered_map<Component*, unique_ptr<Component>> components;
        GameObject(IGameObjectParent* parent);
        void DoThingsOnParents(function<void(GameObject*)> todo);
        // ������͸� ���� ����Լ�
        static int GetSceneIndex(const GameObject* target);
    protected:
        unique_ptr<YunutyEngine::GameObject> MoveChild(GameObject* child) override;
        void ReceiveChild(remove_reference<unique_ptr<GameObject>>::type&& child) override;
        int GetChildIndex(const GameObject* child)const override;

        friend Scene;
        friend YunutyCycle;
#if _DEBUG
    public:
        static int messyIndexingCalled;
#endif
    };
}
